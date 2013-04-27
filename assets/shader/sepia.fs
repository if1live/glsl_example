precision mediump float;
varying vec2 v_texcoord;
varying vec4 v_position;
uniform sampler2D s_scene;

void main() {
	vec4 tex = texture2D(s_scene, v_texcoord);
	vec4 sepia = vec4(0.0, 0.0, 0.0, tex.a);
	
	sepia.x = dot(tex.xyz, vec3(0.393, 0.769, 0.189));
	sepia.y = dot(tex.xyz, vec3(0.349, 0.686, 0.168));
	sepia.z = dot(tex.xyz, vec3(0.272, 0.534, 0.131));
	gl_FragColor = sepia;
}