#pragma once

#include "shader_factory.h"

namespace wv {

class GLSLFactory : public Shader::Factory
{
public:
	virtual std::string build() override;

private:

	Shader::TypeDecl _getRealVertexInputType( const Shader::TypeDecl& _input );
	std::string _buildVertexPullFunction( const Shader::TypeDecl& _input );
	std::string _formatArg( const std::string& _arg, const std::string& _type );
	std::string _formatOutput( const std::string& _arg );

	std::string _buildVertexInput();
	std::string _buildOutput();
	std::string _buildFragments();
	std::string _buildMain();

	const std::string m_vertexBufferName = "u_vertices";
	
	const std::string m_vertexBuffer =
		"layout( std430 ) buffer SbVertices\n"
		"{\n"
		"\tvertex_t u_vertices[];\n"
		"};\n";

	const std::string m_outVertex =
		"out gl_PerVertex\n"
		"{\n"
		"\tvec4 gl_Position;\n"
		"};\n";
};

}
