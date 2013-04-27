attribute vec4 a_position;
attribute vec3 a_normal;

uniform mat4 u_mvp;
uniform mat4 u_lightMVP;
uniform mat4 u_world;

uniform vec3 u_lightPos;

varying vec4 v_shadowCoord;
varying float v_diffuse;

void main() 
{
	v_shadowCoord = u_lightMVP * a_position;
	gl_Position = u_mvp * a_position;
	//gl_Position = u_lightMVP * a_position;
	
	//light
	vec4 worldPos = u_world * a_position;
	vec3 lightDir = normalize(worldPos.xyz - u_lightPos.xyz);
	vec3 worldNormal = normalize(mat3(u_world) * a_normal);
	v_diffuse = dot(-lightDir, worldNormal);
}
