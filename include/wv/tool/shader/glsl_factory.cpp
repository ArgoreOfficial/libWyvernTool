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

	std::string src;
	
	// build header
	src += "#version 460\n";
	src += "#extension GL_ARB_bindless_texture : require\n";
	src += "#extension GL_ARB_shader_draw_parameters : enable\n\n";
	
	src += _buildInput();
	src += _buildVertexInput();
	src += _buildFragments();
	src += _buildOutput();
	src += _buildMain();

	reset();

	return src;
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

	func += wv::format( "%s get%s(int _idx) {\n", _input.type, _input.name );
	func += wv::format( "\treturn %s(\n", _input.type );

	for( size_t i = 0; i < v.count; i++ )
		func += wv::format( "\t\t%s[_idx].%s[%i]%s", m_vertexBufferName, _input.name, i, i == v.count - 1 ? "\n" : ",\n" );

	func += "\t);\n";
	func += "}\n";

	return func;
}

std::string wv::GLSLFactory::_formatArg( const std::string& _arg, const std::string& _type )
{
	if( _arg.empty() || ( _arg[ 0 ] != '$' && _arg[ 0 ] != '#' ) )
		return _arg;

	std::string substr = _arg.substr( 2, _arg.size() - 3 );
	if( _arg[ 0 ] == '$' )
		return wv::format( "get%s(gl_VertexID)", substr.c_str() );
	else if( _arg[ 0 ] == '#' )
		return wv::format( "%s(%s)", _type.c_str(), substr.c_str() );
	return _arg;
}

std::string wv::GLSLFactory::_formatOutput( const std::string& _arg )
{
	if( _arg.empty() || _arg[ 0 ] != '$' )
		return _arg;

	std::string substr = _arg.substr( 2, _arg.size() - 3 );
	
	if( substr == "Position" )
		return "gl_Position";
	
	return _arg;
}

std::string wv::GLSLFactory::_buildInput()
{
	std::string res;

	if( m_stage == Shader::kVertex )
	{
		res += m_cameraData;
		res += m_instanceData;
		res += m_instanceFunctions;
		m_fragmentReturnTypes.insert( { "getAlbedoSampler", "sampler2D" } );
	}

	for( auto& in : m_inputs )
	{
		Shader::ShaderInputOutput v = in.second;
		if( v.bindpoint == -1 )
			res += wv::format( "in %s %s;\n", v.type.c_str(), in.first.c_str() );
		else
			res += wv::format( "layout(location = %i) out %s %s;\n", v.bindpoint, v.type.c_str(), in.first.c_str());
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
			vertexStruct += wv::format( "\t%s %s;\n", v.type.c_str(), v.name.c_str() );
		else
			vertexStruct += wv::format( "\t%s %s[%" PRIu32 "];\n", v.type.c_str(), v.name.c_str(), v.count );
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
			res += wv::format( "out %s %s;\n", v.type.c_str(), out.first.c_str() );
		else
			res += wv::format( "layout(location = %i) out %s %s;\n", v.bindpoint, v.type.c_str(), out.first.c_str());
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
	std::string res;

	res += "void main() {\n";
	for( auto& f : m_executionFunctions )
	{
		std::string func = _formatArg( f.name, "" );
		if( m_fragmentReturnTypes.count( f.name ) == 0 )
		{
			res += wv::format( "\t/* function %s not defined */\n", f.name.c_str() );
			continue;
		}
		std::string type = m_fragmentReturnTypes.at( f.name );
		
		if( f.name[ 0 ] == '$' )
			res += wv::format( "\t%s %s = %s;\n", type.c_str(), f.returnName.c_str(), f.name.c_str() );
		else
		{
			std::string args = " ";
			for( size_t i = 0; i < f.args.size(); i++ )
			{
				std::string arg = _formatArg( f.args[ i ], "" );
				args += wv::format( "%s%s", arg.c_str(), i == f.args.size() - 1 ? " " : ", ");
			}
			res += wv::format( "\t%s %s = %s(%s);\n", type.c_str(), f.returnName.c_str(), f.name.c_str(), args.c_str() );
		}
	}

	for( auto& f : m_outputValues )
	{
		std::string name = _formatOutput( f.first );
		std::string type = "";
		if( m_outputs.count( name ) )
			type = m_outputs.at( name ).type;

		std::string value = _formatArg( f.second, type );
		if( value == "" )
			value = wv::format( "%s(0)", type );

		res += wv::format( "\t%s = %s;\n", name.c_str(), value.c_str() );
	}

	res += "}\n";

	return res;
}
