#version 460 core

in vec3 Normal;
in vec3 FragPosition;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPosition);
	vec3 viewDir = normalize(vec3(0.0) - FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);

	// Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular
	float specularStrenght = 0.5;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 result = (ambient + diffuse + spec) * objectColor;
	FragColor = vec4(result, 1.0);
};