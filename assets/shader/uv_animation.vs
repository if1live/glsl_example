attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform vec3 u_modelLightPos;
uniform vec3 u_modelCameraPos;
uniform float u_elapsedTime;

uniform float u_waveHeight;
uniform float u_speed;
uniform float u_waveFreq;
uniform float u_uvSpeed;

varying vec2 v_texcoord;
varying vec3 v_diffuse;
varying vec3 v_reflection;
varying vec3 v_viewDir;


void main() {
	vec4 pos = a_position;
	float cosTime = u_waveHeight * cos(u_elapsedTime * u_speed + a_texcoord.x * u_waveFreq);
	pos.y += cosTime;

	vec4 modelPos = u_model * pos;
	gl_Position = u_proj * u_view * modelPos;

		
	vec3 lightDir = modelPos.xyz - u_modelLightPos;
	lightDir = normalize(lightDir);
	
	vec3 viewDir = normalize(modelPos.xyz - u_modelCameraPos);
	v_viewDir = viewDir;

	vec3 modelNormal = mat3(u_model) * a_normal;
	modelNormal = normalize(modelNormal);
	
	v_diffuse = dot(-lightDir, modelNormal);
	v_reflection = reflect(lightDir, modelNormal);
	
	v_texcoord = a_texcoord + vec2(u_elapsedTime * 0.25, 0);
}
