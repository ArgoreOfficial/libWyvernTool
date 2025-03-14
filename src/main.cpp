#include <stdio.h>
#include <string>
#include <vector>

#include <wv/tool/shader/glsl_factory.h>

std::string generateVertShader();
std::string generateFragShader();

int main()
{
	std::string vertSource = generateVertShader();
	std::string fragSource = generateFragShader();
	printf( "================ test.vert ================\n" );
	printf( "%s\n", vertSource.c_str() );
	printf( "================ test.frag ================\n" );
	printf( "%s\n", fragSource.c_str() );
	printf( "===========================================\n" );

	return 0;
}

std::string generateVertShader()
{
	wv::GLSLFactory factory{};
	factory.setStage( wv::Shader::kVertex );
	factory.addVertexInput( "vec3", "Position" );
	factory.addVertexInput( "vec3", "Normal" );
	factory.addVertexInput( "vec3", "Tangent" );
	factory.addVertexInput( "vec4", "Color" );
	factory.addVertexInput( "vec2", "TexCoord0" );

	factory.addOutput( "vec2", "TexCoord" );
	factory.addOutput( "vec3", "Normal" );
	factory.addOutput( "vec4", "Position" );

	// GLSL Specific
	std::string lightMultFunction =
		"float light_mult(vec3 _normal, vec3 _lightdir) {\n"
		"	return dot( _normal, _lightdir ) * 0.5 + 0.5;\n"
		"}";

	factory.addFragment( "float", "light_mult", lightMultFunction );
	factory.addFragment( "vec4", "world_to_screen", "#define world_to_screen(_point) u_Projection * u_View * u_Model * vec4(_point, 1.0)" );

	// void main() {
	factory.addFunction( "position", "world_to_screen", { "${Position}" } );

	factory.setOutputValue( "TexCoord",    "${TexCoord0}" );
	factory.setOutputValue( "Normal",      "#{0.0}"       ); 
	factory.setOutputValue( "${Position}", "position"     );
	// }

	return factory.build();
}

std::string generateFragShader()
{
	wv::GLSLFactory factory;
	factory.setStage( wv::Shader::kFragment );
	factory.addInput( "vec2", "TexCoord" );
	factory.addInput( "vec3", "Normal" );
	factory.addInput( "vec4", "Position" );

	factory.addOutput( "vec4", "Albedo",            0 );
	factory.addOutput( "vec2", "Normal",            1 );
	factory.addOutput( "vec2", "RoughnessMetallic", 2 );

	// void main() {
	factory.setOutputValue( "Albedo",            "#{0.0}" );
	factory.setOutputValue( "Normal",            "Normal" );
	factory.setOutputValue( "RoughnessMetallic", "#{1.0}" );
	// }

	return factory.build();
}
