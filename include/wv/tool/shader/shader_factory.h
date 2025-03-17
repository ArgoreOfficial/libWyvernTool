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

struct ShaderInputOutput
{
	std::string type;
	bool flat = false; // if should be interpolated or not
	int32_t bindpoint = -1;
};

enum Stage
{
	kNone = 0,
	kVertex,
	kFragment
};

class Factory
{
public:
	void setStage( Shader::Stage _stage );
	void addVertexInput( std::string _type, std::string _name, uint32_t _count = 1 );
	void addInput( std::string _type, std::string _name, bool _flat = false, int32_t bindpoint = -1 );
	void addOutput( std::string _type, std::string _name, bool _flat = false, int32_t bindpoint = -1 );

	void addFragment( const std::string& _returnType, const std::string& _name, const std::string& _body );
	void loadFragment( const std::string& _filename );

	void addFunction( const std::string& _returnName, const std::string& _name, const std::vector<std::string>& _args = {} );

	void setOutputValue( const std::string& _out, const std::string& _value );

	virtual void reset();
	virtual std::string build() = 0;
	virtual std::string getFragmentsDir() = 0;
	virtual std::string getFragmentExtension() = 0;

protected:
	Shader::Stage m_stage = Shader::kNone;

	std::vector<TypeDecl> m_vertexInput;
	std::unordered_map<std::string, ShaderInputOutput> m_inputs;
	std::unordered_map<std::string, ShaderInputOutput> m_outputs;

	std::unordered_map<std::string, std::string> m_identifiers; // m_identifiers["name"] == "type"

	std::vector<std::string> m_fragments;
	std::vector<Shader::Function> m_executionFunctions;
	std::vector<std::pair<std::string, std::string>> m_outputValues;
};

}


}
