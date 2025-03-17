#include "glsl_factory.h"

#include <wv/tool/string.h>
#include <inttypes.h>

std::string wv::GLSLFactory::build()
{
	if( m_stage == Shader::kNone )
	{
		// error
		return "";
	}

	_parseIdentifiers();

	// core fragments
	loadFragment( "core/texture" );
	loadFragment( "core/vec2" );
	loadFragment( "core/vec3" );
	loadFragment( "core/vec4" );

	if( m_stage == Shader::kVertex )
	{
		loadFragment( "core/vert/AlbedoSampler" );
		loadFragment( "core/vert/ModelMatrix" );
		loadFragment( "core/vert/HasAlpha" );
	}

	std::string src;
	
	// build header
	src += "#version 460\n";
	src += "#extension GL_ARB_bindless_texture : require\n";
	src += "#extension GL_ARB_shader_draw_parameters : enable\n\n";
	
	src += _buildInput();
	src += _buildVertexInput();
	src += _buildOutput();
	src += _buildFragments();
	src += _buildMain();

	reset();

	return src;
}

void wv::GLSLFactory::_parseIdentifiers()
{
	for( auto v : m_vertexInput )
	{
		std::string identifier = wv::format( "get%s", v.name.c_str() );
		m_identifiers[ identifier ] = v.type;
	}
}

wv::Shader::TypeDecl wv::GLSLFactory::_getRealVertexInputType( const Shader::TypeDecl& _input )
{
	wv::Shader::TypeDecl real = _input;
	
	if( _input.type == "vec2" )
	{
		real.type = "float";
		real.count = 2;
	}
	else if( _input.type == "vec3" )
	{
		real.type = "float";
		real.count = 3;
	}
	else if( _input.type == "vec4" )
	{
		real.type = "float";
		real.count = 4;
	}

	return real;
}

std::string wv::GLSLFactory::_buildVertexPullFunction( const Shader::TypeDecl& _input )
{
	std::string func;

	Shader::TypeDecl v = _getRealVertexInputType( _input ); // for real component count

	func += wv::format( "%s get%s() {\n", _input.type, _input.name );
	func += wv::format( "    return %s(\n", _input.type );

	for( size_t i = 0; i < v.count; i++ )
		func += wv::format( "        %s[gl_VertexID].%s[%i]%s", m_vertexBufferName, _input.name, i, i == v.count - 1 ? "\n" : ",\n" );

	func += "    );\n";
	func += "}\n";

	return func;
}

std::string wv::GLSLFactory::_evaluateTypename( const std::string& _arg, const std::string& _type, bool _onlyIdentifier )
{
	if( _arg.empty() || ( _arg[ 0 ] != '$' && _arg[ 0 ] != '#' ) )
		return _arg;

	std::string substr = _arg.substr( 2, _arg.size() - 3 );
	if( _arg[ 0 ] == '$' )
	{
		// check X() identifier
		if( m_identifiers.count( substr ) != 0 )
		{
			if( _onlyIdentifier )
				return wv::format( "%s", substr.c_str() );
			else
				return wv::format( "%s()", substr.c_str() );
		}
		// check getX() identifier
		else if( m_identifiers.count( "get" + substr ) != 0 )
		{
			if( _onlyIdentifier )
				return wv::format( "get%s", substr.c_str() );
			else
				return wv::format( "get%s()", substr.c_str() );
		}
		else
			return _evaluateConstTypename( _arg );
	}
	else if( _arg[ 0 ] == '#' )
		return wv::format( "%s(%s)", _type.c_str(), substr.c_str() );
	return _arg;
}

std::string wv::GLSLFactory::_evaluateConstTypename( const std::string& _arg )
{
	if( _arg.size() < 3 || _arg[ 0 ] != '$' )
		return _arg;

	std::string substr = _arg.substr( 2, _arg.size() - 3 );
	if( m_constTypenames.count( substr ) == 0 )
		return "/*error: " + substr + " not defined*/"; // not a const

	return m_constTypenames.at( substr );
}

std::string wv::GLSLFactory::_buildInput()
{
	std::string res;

	if( m_stage == Shader::kVertex )
	{
		res += m_cameraData;
		res += m_instanceData;
	}

	for( auto& in : m_inputs )
	{
		Shader::ShaderInputOutput v = in.second;
		if( v.bindpoint == -1 )
			res += wv::format( "in %s%s %s;\n", v.flat ? "flat " : "", v.type.c_str(), in.first.c_str() );
		else
			res += wv::format( "layout(location = %i) in %s%s %s;\n", v.flat ? "flat " : "", v.bindpoint, v.type.c_str(), in.first.c_str());
	}
	if( !m_inputs.empty() )
		res += "\n";

	return res;
}

std::string wv::GLSLFactory::_buildVertexInput()
{
	if( m_vertexInput.size() == 0 )
		return "";

	std::string vertexStruct;
	vertexStruct += wv::format( "struct vertex_t {\n" );
	for( auto& input : m_vertexInput )
	{
		Shader::TypeDecl v = _getRealVertexInputType( input );
		if( v.count == 1 )
			vertexStruct += wv::format( "    %s %s;\n", v.type.c_str(), v.name.c_str() );
		else
			vertexStruct += wv::format( "    %s %s[%" PRIu32 "];\n", v.type.c_str(), v.name.c_str(), v.count );
	}
	vertexStruct += wv::format( "};\n\n" );

	std::string vertexPullFuncs;
	for( auto& input : m_vertexInput )
		vertexPullFuncs += _buildVertexPullFunction( input );
	
	std::string vertexInput;
	vertexInput += vertexStruct;
	vertexInput += m_vertexBuffer;
	vertexInput += vertexPullFuncs;
	return vertexInput;
}

std::string wv::GLSLFactory::_buildOutput()
{
	std::string res;
	
	if( m_stage == Shader::kVertex )
		res += m_outVertex;

	for( auto& out : m_outputs )
	{
		Shader::ShaderInputOutput v = out.second;
		if( v.bindpoint == -1 )
			res += wv::format( "out %s%s %s;\n", v.flat ? "flat " : "", v.type.c_str(), out.first.c_str() );
		else
			res += wv::format( "layout(location = %i) out %s%s %s;\n", v.bindpoint, v.flat ? "flat " : "", v.type.c_str(), out.first.c_str());
	}
	if( !m_outputs.empty() )
		res += "\n";
	
	return res;
}

std::string wv::GLSLFactory::_buildFragments()
{
	std::string res;
	for( auto& f : m_fragments )
		res += f;

	return res;
}

std::string wv::GLSLFactory::_buildMain()
{
	std::string res = "\n// generated main\n";
	res += "void main() {\n";

	for( auto& f : m_executionFunctions )
	{
		std::string func = _evaluateTypename( f.name, "", true );
		if( m_identifiers.count( func ) == 0 )
		{
			res += wv::format( "    /* function %s not defined */\n", f.name.c_str() );
			continue;
		}
		std::string type = m_identifiers.at( func );
		
		if( f.name[ 0 ] == '$' )
			res += wv::format( "    %s %s = %s();\n", type.c_str(), f.returnName.c_str(), func.c_str() );
		else
		{
			std::string args = "";
			for( size_t i = 0; i < f.args.size(); i++ )
			{
				std::string arg = _evaluateTypename( f.args[ i ], "" );
				args += wv::format( "%s%s", arg.c_str(), i == f.args.size() - 1 ? "" : ", ");
			}
			res += wv::format( "    %s %s = %s(%s);\n", type.c_str(), f.returnName.c_str(), f.name.c_str(), args.c_str() );
		}
	}

	for( auto& f : m_outputValues )
	{
		std::string name = _evaluateConstTypename( f.first );
		std::string type = "";
		if( m_outputs.count( name ) )
			type = m_outputs.at( name ).type;

		std::string value = _evaluateTypename( f.second, type );
		if( value == "" )
			value = wv::format( "%s(0)", type );

		res += wv::format( "    %s = %s;\n", name.c_str(), value.c_str() );
	}

	res += "}\n";

	return res;
}
