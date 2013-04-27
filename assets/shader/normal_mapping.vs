attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
attribute vec3 a_tangent;

uniform mat4 u_mvp;
uniform mat4 u_model;

uniform vec3 u_modelLightPos;
uniform vec3 u_modelCameraPos;

varying vec2 v_texcoord;
varying vec3 v_viewDir;
varying vec3 v_lightDir;
varying vec3 v_T;
varying vec3 v_B;
varying vec3 v_N;

void main() {
	gl_Position = u_mvp * a_position;

	vec4 modelPos = u_model * a_position;
	vec3 lightDir = modelPos.xyz - u_modelLightPos;
	v_lightDir = normalize(lightDir);
	
	vec3 viewDir = normalize(modelPos.xyz - u_modelCameraPos);
	v_viewDir = viewDir;

	vec3 modelNormal = mat3(u_model) * a_normal;
	v_N = normalize(modelNormal);
	
	vec3 modelTangent = mat3(u_model) * a_tangent;
	v_T = normalize(modelTangent);
	
	vec3 modelBinormal = cross(v_N, v_T);
	v_B = normalize(modelBinormal);
	
	v_texcoord = a_texcoord;
}
