precision mediump float;
varying vec2 v_texcoord;
varying vec4 v_position;
uniform sampler2D s_scene;

void main() {
	gl_FragColor = texture2D(s_scene, v_texcoord);
}