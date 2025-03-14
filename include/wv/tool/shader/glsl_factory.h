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

	std::string _buildInput();
	std::string _buildVertexInput();
	std::string _buildOutput();
	std::string _buildFragments();
	std::string _buildMain();

	const std::string m_vertexBufferName = "u_vertices";
	
	const std::string m_vertexBuffer =
		"layout( std430 ) buffer SbVertices\n"
		"{\n"
		"	vertex_t u_vertices[];\n"
		"};\n";

	const std::string m_outVertex =
		"out gl_PerVertex\n"
		"{\n"
		"	vec4 gl_Position;\n"
		"};\n";

	const std::string m_cameraData =
		"uniform UbCameraData\n"
		"{\n"
		"	mat4x4 u_Projection;\n"
		"	mat4x4 u_View;\n"
		"	mat4x4 u_Model;\n"
		"};\n";
		
	const std::string m_instanceData =
		"struct sInstance\n"
		"{\n"
		"	mat4x4 Model;\n"
		"	uvec2 TextureHandles[ 4 ];\n"
		"	int HasAlpha;\n"
		"	int padding0[ 3 ];\n"
		"};\n"
		"\n"
		"layout( std430 ) buffer SbInstances\n"
		"{\n"
		"	sInstance u_instances[];\n"
		"};\n";

	const std::string m_instanceFunctions =
		"sampler2D getAlbedoSampler( int _idx ) {\n"
		"    return sampler2D( u_instances[ _idx ].TextureHandles[ 0 ] );\n"
		"}\n";

};

}
