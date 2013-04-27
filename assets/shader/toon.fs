precision mediump float;

uniform vec3 u_lightColor;

varying vec3 v_diffuse;

void main() {
	vec3 diffuse = clamp(v_diffuse, 0.0, 1.0);
	diffuse = ceil(diffuse * 5.0) / 5.0;
	gl_FragColor = vec4(u_lightColor * diffuse, 1.0);
}