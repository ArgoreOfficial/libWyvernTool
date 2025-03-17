#include "shader_factory.h"
#include <fstream>
#include <sstream>
#include <string>


void wv::Shader::Factory::setStage( Shader::Stage _stage )
{
	m_stage = _stage;
}

void wv::Shader::Factory::addVertexInput( std::string _type, std::string _name, uint32_t _count )
{
	m_vertexInput.push_back( { _type, _name, _count } );
}

void wv::Shader::Factory::addInput( std::string _type, std::string _name, int32_t bindpoint )
{
	m_inputs[ _name ] = { bindpoint, _type };
}

void wv::Shader::Factory::addOutput( std::string _type, std::string _name, int32_t bindpoint )
{
	m_outputs[ _name ] = { bindpoint, _type };
}

void wv::Shader::Factory::addFragment( const std::string& _returnType, const std::string& _name, const std::string& _body )
{
	m_fragments.push_back( _body + "\n" );
	m_identifiers[ _name ] = _returnType;
}

void wv::Shader::Factory::loadFragment( const std::string& _filename )
{
	std::ifstream infile( getFragmentsDir() + _filename + getFragmentExtension() );
	std::string line;

	std::string returntype;
	std::string name;
	std::string body;
	
	std::getline( infile, returntype );
	std::getline( infile, name );

	while( std::getline( infile, line ) )
		body += line + "\n";
	
	addFragment( returntype, name, body );
}

void wv::Shader::Factory::addFunction( const std::string& _returnName, const std::string& _name, const std::vector<std::string>& _args )
{
	m_executionFunctions.push_back( { _returnName, _name, _args } );
}

void wv::Shader::Factory::setOutputValue( const std::string& _out, const std::string& _value )
{
	m_outputValues.push_back( { _out, _value } );
}

void wv::Shader::Factory::reset()
{
	m_vertexInput.clear();
	m_outputs.clear();

	m_fragments.clear();
	m_executionFunctions.clear();
	m_outputValues.clear();
}

