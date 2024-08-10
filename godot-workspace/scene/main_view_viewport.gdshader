shader_type spatial;

uniform sampler2D _viewport_texture;

void fragment() {
	vec4 _col_tex = texture(_viewport_texture, UV);
	
	ALBEDO = _col_tex.xyz;
	ALPHA = _col_tex.a;
}
