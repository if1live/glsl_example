precision mediump float;
varying vec2 v_texcoord;
varying vec4 v_position;

uniform sampler2D s_scene;
uniform vec2 u_pixelOffset;

void main() 
{
	float res = 0;
	
	mat3 K = mat3(
	-2, -1, 0, 
	-1, 0, 1,
	0, 1, 2
	);
	
	for(int y = -1 ; y <= 1 ; ++y) {
		for(int x = -1 ; x <= 1 ; ++x) {
			vec2 offset = vec2(x, y) * u_pixelOffset;
			vec3 tex = texture2D(s_scene, v_texcoord + offset).xyz;
			float luminance = dot(tex, vec3(0.3, 0.59, 0.11));
			
			res += luminance * K[y+1][x+1];
		}
	}

	res += 0.5;
	gl_FragColor = vec4(res.xxx, 1.0);
}