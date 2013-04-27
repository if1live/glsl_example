precision mediump float;
varying vec3 v_diffuse;
varying vec3 v_reflection;
varying vec3 v_viewDir;

void main() {
	vec3 diffuse = clamp(v_diffuse, 0.0, 1.0);
	
	vec3 reflection = normalize(v_reflection);
	vec3 viewDir = normalize(v_viewDir);
	vec3 specular = vec3(0.0, 0.0, 0.0);
	
	// vertex shader에서 기본 빛 계산을 하는것과 겹치면 
	// 빛이 뒤로 넘어가는 경계에서 깨질수 있다. diffuse=0인데 specular가 잇을수잇다
	// 그래서 항상 계산하도록함
	if(diffuse.x >= 0.0) {
		float specularValue = 0;
		specularValue = clamp(dot(reflection, -viewDir), 0.0, 1.0);
		specularValue = pow(specularValue, 20.0);
		
		specular = vec3(specularValue);
	}
	vec3 ambient = vec3(0.1, 0.1, 0.1);
	
	gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
	//gl_FragColor = vec4(specular, 1.0);
}