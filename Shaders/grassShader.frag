#version 430 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 vertexColor;

uniform vec3 viewPos;

void main()
{

	// Static sun (TODO: Change)
	vec3 lightPos = vec3(0.0, 4.0, 0.0);
	vec3 lightColor = vec3(1.0, 0.95, 0.8);

	// Ambient
	float ambientI = 0.6;
	vec3 ambient = ambientI * lightColor;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// For the inner part (because of the bend) we invert the directions of the normals
	if (!gl_FrontFacing) {
		norm = -norm;
	}
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;


	//Specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 5);
	vec3 specular = specularStrength * spec * lightColor;


	vec3 result = (ambient + diffuse + specular) * vertexColor;
	FragColor = vec4(result, 1.0);

}