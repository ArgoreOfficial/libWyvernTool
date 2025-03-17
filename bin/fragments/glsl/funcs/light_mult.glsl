float
light_mult
// fragments/funcs/light_mult.glsl
float light_mult(vec3 _normal, vec3 _lightdir) {
	return dot( _normal, _lightdir ) * 0.5 + 0.5;
}