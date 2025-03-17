vec4
world_to_clip4
// fragments/inline/world_to_clip4.glsl
#define world_to_clip4(_point,_model) u_Projection * u_View * _model * vec4(_point, 1.0 )