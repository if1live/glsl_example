attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform vec3 u_modelLightPos;
uniform vec3 u_modelCameraPos;

varying vec2 v_texcoord;
varying vec3 v_diffuse;
varying vec3 v_reflection;
varying vec3 v_viewDir;


void main() {
	vec4 modelPos = u_model * a_position;
	gl_Position = u_proj * u_view * modelPos;

		
	vec3 lightDir = modelPos.xyz - u_modelLightPos;
	lightDir = normalize(lightDir);
	
	vec3 viewDir = normalize(modelPos.xyz - u_modelCameraPos);
	v_viewDir = viewDir;

	vec3 modelNormal = mat3(u_model) * a_normal;
	modelNormal = normalize(modelNormal);
	
	v_diffuse = dot(-lightDir, modelNormal);
	v_reflection = reflect(lightDir, modelNormal);
	
	v_texcoord = a_texcoord;
}
