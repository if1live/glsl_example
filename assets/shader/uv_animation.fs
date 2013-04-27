precision mediump float;
varying vec3 v_diffuse;
varying vec3 v_reflection;
varying vec3 v_viewDir;
varying vec2 v_texcoord;

uniform sampler2D s_diffuse;
uniform sampler2D s_specular;

uniform vec4 u_lightColor;

void main() {
	vec4 albedo = texture2D(s_diffuse, v_texcoord);
	vec3 diffuse = clamp(albedo.xyz * v_diffuse * u_lightColor.xyz, 0.0, 1.0);
	
	vec3 reflection = normalize(v_reflection);
	vec3 viewDir = normalize(v_viewDir);
	vec3 specular = vec3(0.0, 0.0, 0.0);
	if(diffuse.x >= 0.0) {
		float specularValue = 0;
		specularValue = clamp(dot(reflection, -viewDir), 0.0, 1.0);
		specularValue = pow(specularValue, 20.0);
		
		vec4 specularIntensity = texture2D(s_specular, v_texcoord);
		specular = specularIntensity.xyz * vec3(specularValue) * u_lightColor.xyz;
	}
	vec3 ambient = vec3(0.1, 0.1, 0.1) * albedo.xyz;
	
	gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}