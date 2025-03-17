vec4
world_to_screen
// fragments/inline/world_to_screen.glsl
#define world_to_screen(_point) u_Projection * u_View * u_Model * vec4(_point, 1.0)