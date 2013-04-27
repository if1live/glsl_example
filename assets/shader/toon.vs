attribute vec4 a_position;
attribute vec3 a_normal;

uniform mat4 u_mvp;
uniform mat4 u_invModel;
uniform vec3 u_modelLightPos;

varying vec3 v_diffuse;

void main() {
	gl_Position = u_mvp * a_position;
	
	vec3 objectLightPos = mat3(u_invModel) * u_modelLightPos;
	vec3 lightDir = normalize(a_position.xyz - objectLightPos);
	
	v_diffuse = dot(-lightDir, a_normal);
}
