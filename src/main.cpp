#include <stdio.h>
#include <string>
#include <vector>

#include <wv/tool/shader/glsl_factory.h>
#include <fstream>

std::string generateBasicVertShader();
std::string generateBasicFragShader();
std::string generateVulkanFragShader();

int main()
{
	std::ofstream vs( "generated/basic_vs.glsl" );
	vs << generateBasicVertShader();

	std::ofstream fs( "generated/basic_fs.glsl" );
	fs << generateBasicFragShader();

	std::ofstream fs( "generated/vk_fs.glsl" );
	fs << generateVulkanFragShader();

	return 0;
}

std::string generateBasicVertShader()
{
	wv::GLSLFactory factory{};
	factory.setStage( wv::Shader::kVertex );
	factory.addVertexInput( "vec3", "Position" );
	factory.addVertexInput( "vec3", "Normal" );
	factory.addVertexInput( "vec3", "Tangent" );
	factory.addVertexInput( "vec4", "Color" );
	factory.addVertexInput( "vec2", "TexCoord0" );

	factory.addOutput( "vec2", "out_TexCoord" );
	factory.addOutput( "vec3", "out_Normal"   );
	factory.addOutput( "vec3", "out_Position" );
	factory.addOutput( "int",  "out_InstanceID",  true );
	factory.addOutput( "int",  "out_HasAlpha",    true );
	factory.addOutput( "sampler2D", "out_Albedo", true );

	factory.loadFragment( "inline/world_to_view3" );
	factory.loadFragment( "inline/world_to_clip4" );
	factory.loadFragment( "inline/tangent_to_world3" );
	factory.loadFragment( "funcs/light_mult" );

	// void main() {
	factory.addFunction( "model", "${ModelMatrix}" );
	factory.addFunction( "view_pos", "world_to_view3", { "${Position}", "model" });
	factory.addFunction( "clip_pos", "world_to_clip4", { "${Position}", "model" });
	factory.addFunction( "world_norm", "tangent_to_world3", { "${Normal}", "model" });

	factory.setOutputValue( "out_InstanceID", "${InstanceID}" );
	factory.setOutputValue( "out_Albedo",     "${AlbedoSampler}" );
	factory.setOutputValue( "out_HasAlpha",   "${HasAlpha}" );
	factory.setOutputValue( "out_TexCoord",   "${TexCoord0}" );
	factory.setOutputValue( "out_Normal",     "world_norm" );
	factory.setOutputValue( "out_Position",	  "view_pos" );
	factory.setOutputValue( "${OutPosition}", "clip_pos" );
	// }

	return factory.build();
}

std::string generateBasicFragShader()
{
	wv::GLSLFactory factory;
	factory.setStage( wv::Shader::kFragment );
	factory.addInput( "vec2", "in_TexCoord" );
	factory.addInput( "vec3", "in_Normal" );
	factory.addInput( "vec4", "in_Position" );
	factory.addInput( "int ", "in_InstanceID",  true );
	factory.addInput( "int",  "in_HasAlpha",    true );
	factory.addInput( "sampler2D", "in_Albedo", true );

	factory.addOutput( "vec4", "out_Albedo",            false, 0 );
	factory.addOutput( "vec2", "out_Normal",            false, 1 );
	factory.addOutput( "vec2", "out_RoughnessMetallic", false, 2 );
	
	factory.loadFragment( "funcs/encode_normal_oct" );

	// void main() {
	factory.addFunction( "color", "texture", { "in_Albedo","in_TexCoord" } );
	factory.addFunction( "normal", "encode_normal_oct", { "in_Normal" } );
	factory.setOutputValue( "out_Albedo", "color" );
	factory.setOutputValue( "out_Normal", "normal" );
	factory.setOutputValue( "out_RoughnessMetallic", "#{1.0}" );
	// }

	return factory.build();
}

std::string generateVulkanFragShader()
{
	wv::GLSLFactory factory;
	factory.setStage( wv::Shader::kFragment );
	factory.addInput( "int",    "inDrawID",      true,  0 );
	factory.addInput( "vec4",   "in_Normal",     false, 1 );
	factory.addInput( "vec2",   "in_Position",   false, 2 );
	factory.addInput( "vec2 ",  "in_InstanceID", false, 3 );
	factory.addInput( "vec3",   "in_HasAlpha",   false, 4 );
	factory.addInput( "mat3x3", "in_Albedo",     false, 5 );
	factory.addInput( "int",    "in_Albedo",     true,  6 );

	factory.addOutput( "vec4", "outFragColor", false, 0 );

	// uniforms

	/*

	factory.beginUniformStruct( "u_Material", 0, 1, 255 );
	factory.addUniform( "vec4", "MaterialColor" );
	factory.addUniform( "vec4", "LightColor" );
	factory.addUniform( "bool", "IsVideo" );
	factory.addUniform( "bool", "IsCubemapped" );
	factory.endUniformStruct();

	factory.addUniform( 0, 2, 255, "texture", "u_Colormap"      );
	factory.addUniform( 0, 3, 255, "texture", "u_ColormapY"     );
	factory.addUniform( 0, 4, 255, "texture", "u_ColormapUV"    );
	factory.addUniform( 0, 5, 255, "texture", "u_Normalmap"     );
	factory.addUniform( 0, 6, 255, "texture", "u_Specularmap"   );
	factory.addUniform( 0, 7, 255, "texture", "u_Lightmap"      );
	factory.addUniform( 0, 8, 255, "texture", "u_Emissivemap"   );
	factory.addUniform( 0, 9, 255, "cubemap", "u_Reflectionmap" );



	becomes:



	layout(set = 0, binding = 1) uniform MaterialProperties {
		vec4 MaterialColor;
		vec4 LightColor;
		bool IsVideo;
		bool IsCubemapped;
	} u_Material[];

	layout(set = 0, binding = 2) uniform sampler2D u_Colormap[];
	layout(set = 0, binding = 3) uniform sampler2D u_ColormapY[];
	layout(set = 0, binding = 4) uniform sampler2D u_ColormapUV[];
	layout(set = 0, binding = 5) uniform sampler2D u_Normalmap[];
	layout(set = 0, binding = 6) uniform sampler2D u_Specularmap[];
	layout(set = 0, binding = 7) uniform sampler2D u_Lightmap[];
	layout(set = 0, binding = 8) uniform sampler2D u_Emissivemap[];
	layout(set = 0, binding = 9) uniform samplerCube u_Reflectionmap[];
	*/

	// void main() {
	factory.setOutputValue( "outFragColor", "#{1.0}" );
	// }

	return factory.build();
}
