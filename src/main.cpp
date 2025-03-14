#include <stdio.h>
#include <string>
#include <vector>

#include <wv/tool/shader/glsl_factory.h>

int main()
{
	wv::GLSLFactory glslFactory;
	glslFactory.begin( "test_vs.glsl" );
	glslFactory.addVertexInput( "vec3", "Position"  );
	glslFactory.addVertexInput( "vec3", "Normal"    );
	glslFactory.addVertexInput( "vec3", "Tangent"   );
	glslFactory.addVertexInput( "vec4", "Color"     );
	glslFactory.addVertexInput( "vec2", "TexCoord0" );

	glslFactory.addVertexOutput( "vec2", "TexCoord" );
	glslFactory.addVertexOutput( "vec3", "Normal" );
	glslFactory.addVertexOutput( "vec4", "Position" );

	// GLSL Specific
	std::string lightMultFunction =
		"float light_mult(vec3 _normal, vec3 _lightdir) {\n"
		"	return dot( _normal, _lightdir ) * 0.5 + 0.5;\n"
		"}";

	glslFactory.addFragment( "float", "light_mult", lightMultFunction );
	glslFactory.addFragment( "vec4", "world_to_screen", "#define world_to_screen(_point) u_Projection * u_View * u_Model * vec4(_point, 1.0)" );

	// void main() {
	glslFactory.addFunction( "position", "world_to_screen", { "${Position}" } );

	glslFactory.addOutput( "TexCoord", "${TexCoord0}" );
	glslFactory.addOutput( "Normal",   ""             ); // default
	glslFactory.addOutput( "${Position}", "position"  );
	// }
	
	std::string shaderSource = glslFactory.build();
	printf( "%s\n", shaderSource.c_str() );

	return 0;
}
