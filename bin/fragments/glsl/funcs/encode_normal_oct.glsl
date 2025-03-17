vec2
encode_normal_oct
// encode_normal_oct.glsl
vec2 oct_wrap(vec2 v) {
    vec2 w = 1.0 - abs( v.yx );
    if (v.x < 0.0) w.x = -w.x;
    if (v.y < 0.0) w.y = -w.y;
    return w;
}

vec2 encode_normal_oct(vec3 n) {
    n /= ( abs( n.x ) + abs( n.y ) + abs( n.z ) );
    n.xy = n.z > 0.0 ? n.xy : oct_wrap( n.xy );
    n.xy = n.xy * 0.5 + 0.5;
    return n.xy;
}