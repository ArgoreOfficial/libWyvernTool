#include <stdio.h>
#include <string>
#include <vector>

#include <wv/tool/shader/glsl_factory.h>
#include <fstream>

std::string generateBasicVertShader();
std::string generateBasicFragShader();

int main()
{
	std::ofstream vs( "generated/basic_vs.glsl" );
	vs << generateBasicVertShader();

	std::ofstream fs( "generated/basic_fs.glsl" );
	fs << generateBasicFragShader();

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
	factory.addOutput( "vec3", "out_Normal" );
	factory.addOutput( "vec3", "out_Position" );
	factory.addOutput( "flat int",       "out_InstanceID" );
	factory.addOutput( "flat sampler2D", "out_Albedo" );
	factory.addOutput( "flat int",       "out_HasAlpha" );

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
	factory.addInput( "flat int ",      "in_InstanceID" );
	factory.addInput( "flat sampler2D", "in_Albedo" );
	factory.addInput( "flat int",       "in_HasAlpha" );

	factory.addOutput( "vec4", "out_Albedo", 0 );
	factory.addOutput( "vec2", "out_Normal", 1 );
	factory.addOutput( "vec2", "out_RoughnessMetallic", 2 );
	
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
