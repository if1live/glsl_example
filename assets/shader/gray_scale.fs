precision mediump float;
varying vec2 v_texcoord;
varying vec4 v_position;
uniform sampler2D s_scene;

void main() {
	vec4 tex = texture2D(s_scene, v_texcoord);
	tex.xyz = dot(tex.xyz, vec3(0.3, 0.59, 0.11));
	gl_FragColor = tex;
}