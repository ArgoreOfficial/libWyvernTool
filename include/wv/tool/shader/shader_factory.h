#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <utility>

#include <unordered_map>

namespace wv {

namespace Shader {

struct TypeDecl
{
	std::string type;
	std::string name;
	uint32_t count = 1;
};

struct FunctionDefinition
{
	std::string returnType; 
	std::string name;
	std::string body;
};

struct Function
{
	std::string returnName; 
	std::string name;
	std::vector<std::string> args;
};

class Factory
{
public:
	void begin( const std::string& _name ) {
		m_name = _name;
	}

	void addVertexInput( std::string _type, std::string _name, uint32_t _count = 1 ) {
		m_vertexInput.push_back( { _type, _name, _count } );
	}

	void addVertexOutput( std::string _type, std::string _name ) {
		m_vertexOutput.insert( { _name, _type } );
	}

	void addFragment( const std::string& _returnType, const std::string& _name, const std::string& _body ) {
		m_fragments.push_back( _body + "\n" );
		m_fragmentReturnTypes.insert( { _name, _returnType } );
	}

	void addFunction( const std::string& _returnName, const std::string& _name, const std::vector<std::string>& _args = {} ) {
		m_executionFunctions.push_back( { _returnName, _name, _args } );
	}

	void addOutput( const std::string& _out, const std::string& _value ) {
		m_outputValues.push_back( { _out, _value } );
	}

	virtual void reset();
	virtual std::string build() = 0;

protected:
	std::string m_name = "";

	std::vector<TypeDecl> m_vertexInput;
	std::unordered_map<std::string, std::string> m_vertexOutput;

	std::vector<std::string> m_fragments;
	std::unordered_map<std::string, std::string> m_fragmentReturnTypes;

	std::vector<Shader::Function> m_executionFunctions;
	std::vector<std::pair<std::string, std::string>> m_outputValues;
};

}


}
