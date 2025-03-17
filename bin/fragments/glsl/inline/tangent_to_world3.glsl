vec3
tangent_to_world3
// fragments/inline/tangent_to_world3.glsl
#define tangent_to_world3(_normal,_model) normalize( transpose( inverse( mat3( _model ) ) ) * _normal )