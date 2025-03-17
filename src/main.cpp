#include <stdio.h>
#include <string>
#include <vector>

#include <wv/tool/shader/glsl_factory.h>
#include <wv/svensk.hpp>

std::string generateVertShader();
std::string generateFragShader();

tom g�r_saker som_tar_in stor_siffra _a, bred_karakt�r_t _w som_parametrar d�
	skrivare som_tar_in "_a plus _w �r %i\n", _a plus _w som_parametrar radslut
slut

tom funktion som_inte_tar_n�gra_parametrar d�
	konstant siffra som_inte_kan_g�_minus antal_tr�d �r tolv;
slut

int main()
{
	g�r_saker som_tar_in 1, 4 som_parametrar radslut
	funktion som_inte_tar_n�gra_parametrar radslut

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
	factory.addVertexInput( "vec3", "Position"  );
	factory.addVertexInput( "vec3", "Normal"    );
	factory.addVertexInput( "vec3", "Tangent"   );
	factory.addVertexInput( "vec4", "Color"     );
	factory.addVertexInput( "vec2", "TexCoord0" );

	factory.addOutput( "vec2", "out_TexCoord" );
	factory.addOutput( "vec3", "out_Normal"   );
	factory.addOutput( "vec4", "out_Position" );

	// GLSL Specific
	std::string lightMultFunction =
		"float light_mult(vec3 _normal, vec3 _lightdir) {\n"
		"	return dot( _normal, _lightdir ) * 0.5 + 0.5;\n"
		"}";
	factory.addFragment( "float", "light_mult", lightMultFunction );
	factory.addFragment( "vec4", "world_to_screen", "#define world_to_screen(_point) u_Projection * u_View * u_Model * vec4(_point, 1.0)" );

	// void main() {
	factory.addFunction( "position", "world_to_screen", { "${Position}" } );
	factory.addFunction( "uv", "${TexCoord0}" );

	factory.setOutputValue( "out_TexCoord", "${TexCoord0}" );
	factory.setOutputValue( "out_Normal",   "#{0.0}"       ); 
	factory.setOutputValue( "${Position}",  "position"     );
	// }

	return factory.build();
}

std::string generateFragShader()
{
	wv::GLSLFactory factory;
	factory.setStage( wv::Shader::kFragment );
	factory.addInput( "vec2", "in_TexCoord" );
	factory.addInput( "vec3", "in_Normal"   );
	factory.addInput( "vec4", "in_Position" );

	factory.addOutput( "vec4", "out_Albedo",            0 );
	factory.addOutput( "vec2", "out_Normal",            1 );
	factory.addOutput( "vec2", "out_RoughnessMetallic", 2 );

	// void main() {
	factory.setOutputValue( "out_Albedo",            "#{0.0}"       );
	factory.setOutputValue( "out_Normal",            "in_Normal.xy" );
	factory.setOutputValue( "out_RoughnessMetallic", "#{1.0}"       );
	// }

	return factory.build();
}
