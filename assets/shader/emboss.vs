attribute vec4 a_position;
attribute vec2 a_texcoord;

varying vec4 v_position;
varying vec2 v_texcoord;

void main() {
	v_texcoord = a_texcoord;
	v_position = a_position;
	gl_Position = a_position;
}
