#pragma once

#include "shader_factory.h"

namespace wv {

class GLSLFactory : public Shader::Factory
{
public:
	virtual std::string build() override;
	virtual std::string getFragmentsDir() override { return "fragments/glsl/"; }
	virtual std::string getFragmentExtension() override { return ".glsl"; }

private:

	void _parseIdentifiers();

	Shader::TypeDecl _getRealVertexInputType( const Shader::TypeDecl& _input );
	std::string _buildVertexPullFunction( const Shader::TypeDecl& _input );
	
	std::string _evaluateTypename( const std::string& _arg, const std::string& _type, bool _onlyIdentifier = false );
	std::string _evaluateConstTypename( const std::string& _arg );

	std::string _buildInput();
	std::string _buildVertexInput();
	std::string _buildOutput();
	std::string _buildFragments();
	std::string _buildMain();

	void _loadDefaultVertexIdentifiers();

	const std::string m_vertexBufferName = "u_vertices";
	
	const std::string m_vertexBuffer =
		"layout( std430 ) buffer SbVertices {\n"
		"    vertex_t u_vertices[];\n"
		"};\n";

	const std::string m_outVertex =
		"out gl_PerVertex {\n"
		"    vec4 gl_Position;\n"
		"};\n";

	const std::string m_cameraData =
		"uniform UbCameraData {\n"
		"    mat4x4 u_Projection;\n"
		"    mat4x4 u_View;\n"
		"    mat4x4 u_Model;\n"
		"};\n";
		
	const std::string m_instanceData =
		"struct sInstance {\n"
		"    mat4x4 NodeModel;\n"
		"    mat4x4 ModelModel;\n"
		"    uvec2 TextureHandles[ 4 ];\n"
		"    int HasAlpha;\n"
		"    int padding0[3];\n"
		"};\n"
		"\n"
		"layout( std430 ) buffer SbInstances {\n"
		"    sInstance u_instances[];\n"
		"};\n";

	const std::string m_instanceFunctions =
		"sampler2D getAlbedoSampler() {\n"
		"    return sampler2D( u_instances[gl_InstanceID + gl_BaseInstanceARB].TextureHandles[ 0 ] );\n"
		"}\n"
		"mat4x4 getModelMatrix() {\n"
		"	return u_instances[gl_InstanceID + gl_BaseInstanceARB].ModelModel * u_instances[gl_InstanceID + gl_BaseInstanceARB].NodeModel;\n"
		"}\n"
		"#define getHasAlpha() u_instances[gl_InstanceID + gl_BaseInstanceARB].HasAlpha\n";

	const std::unordered_map<std::string, std::string> m_constTypenames = {
		{ "OutPosition", "gl_Position"  },
		{ "OutColor",    "gl_FragColor" },
		{ "VertexID",    "gl_VertexID"  },
		{ "InstanceID",  "gl_InstanceID + gl_BaseInstanceARB" }
	};
};

}
