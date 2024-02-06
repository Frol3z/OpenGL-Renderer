#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

out vec3 Color;

void main()
{
	// Compute vertex position
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	
	// Normals and vertex in view space
	vec3 norm = mat3(transpose(inverse(view * model))) * vNormal;
	vec3 vertPosition = vec3(view * model * vec4(vPosition, 1.0));

	// Light, view and reflection direction
	vec3 lightDir = normalize(lightPosition - vertPosition);
	vec3 viewDir = normalize(vec3(0.0) - vertPosition);
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
	Color = result;
};