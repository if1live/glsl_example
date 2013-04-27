precision mediump float;

varying vec3 v_normal;
varying vec3 v_viewDir;
varying vec2 v_texcoord;
varying vec3 v_lightDir;

varying vec3 v_T;
varying vec3 v_B;
varying vec3 v_N;

uniform sampler2D s_diffuse;
uniform sampler2D s_specular;
uniform sampler2D s_normal;
uniform samplerCube s_environment;

uniform vec4 u_lightColor;

void main() {
	vec3 tangentNormal = texture2D(s_normal, v_texcoord).xyz;
	tangentNormal = normalize(tangentNormal * 2.0 - 1.0);
	mat3 basis = mat3(normalize(v_T), normalize(v_B), normalize(v_N));
	vec3 modelNormal = basis * tangentNormal;
	
	vec4 albedo = texture2D(s_diffuse, v_texcoord);
	vec3 lightDir = normalize(v_lightDir);
	vec3 diffuse = clamp(dot(-lightDir, modelNormal), 0.0, 1.0);
	diffuse *= albedo.xyz * u_lightColor.xyz;
	
	vec3 viewDir = normalize(v_viewDir);
	vec3 specular = vec3(0.0, 0.0, 0.0);
	if(diffuse.x >= 0.0) {
		vec3 reflection = reflect(lightDir, modelNormal);
		
		float specularValue = clamp(dot(reflection, -viewDir), 0.0, 1.0);
		specularValue = pow(specularValue, 20.0);
		specular = vec3(specularValue);
		
		vec4 specularIntensity = texture2D(s_specular, v_texcoord);
		specular *= specularIntensity.xyz * u_lightColor.xyz;
	}
	vec3 ambient = vec3(0.1, 0.1, 0.1) * albedo.xyz;
	
	vec3 viewReflect = reflect(viewDir, modelNormal);
	vec3 environment = textureCube(s_environment, viewReflect).xyz;
	//gl_FragColor = vec4(ambient + diffuse + specular + environment * 0.5, 1.0);
	//gl_FragColor = vec4(environment, 1.0);
	gl_FragColor = vec4((ambient + diffuse + specular) * 0.5 + environment, 1.0);
}