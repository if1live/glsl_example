precision mediump float;
varying vec2 v_texcoord;
varying vec4 v_position;

uniform sampler2D s_scene;
uniform vec2 u_pixelOffset;

void main() 
{
	float Lx = 0;
	float Ly = 0;
	
	mat3 Kx = mat3(
	-1, 0, 1,
	-2, 0, 2,
	-1, 0, 1	
	);
	
	mat3 Ky = mat3(
	1, 2, 1,
	0, 0, 0,
	-1, -2, -1
	);
	
	for(int y = -1 ; y <= 1 ; ++y) {
		for(int x = -1 ; x <= 1 ; ++x) {
			vec2 offset = vec2(x, y) * u_pixelOffset;
			vec3 tex = texture2D(s_scene, v_texcoord + offset).xyz;
			float luminance = dot(tex, vec3(0.3, 0.59, 0.11));
			
			Lx += luminance * Kx[y+1][x+1];
			Ly += luminance * Ky[y+1][x+1];
		}
	}

	float L = sqrt((Lx*Lx) + (Ly*Ly));
	gl_FragColor = vec4(L.xxx, 1.0);
}