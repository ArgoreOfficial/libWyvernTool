vec3
world_to_view3
// fragments/inline/world_to_view3.glsl
#define world_to_view3(_point,_model) (u_View * _model * vec4(_point, 1.0)).xyz