#version 460 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPosition;

out vec4 FragColor;

struct Material
{
	sampler2D diffuse; // replacing ambient and diffuse
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct DirLight
{
	vec3 direction; // from the light
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material u_material;
uniform DirLight u_dirLight;
uniform float u_time;

vec3 CalcDirLight(DirLight dir, vec3 normal, vec3 viewDir);

void main()
{
	vec3 result = vec3(0.0);

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(vec3(0.0) - FragPosition);

	result += CalcDirLight(u_dirLight, norm, viewDir);

	// @todo Point lights
	// @todo Spot lights
	
	// Emission map
	vec3 emission = ((sin(u_time) + 1) / 2) * texture(u_material.emission, TexCoord).rgb;
	result += emission;

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight dir, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-dir.direction);

	// Ambient
	vec3 ambient = dir.ambient * vec3(texture(u_material.diffuse, TexCoord));

	// Diffuse
	float diff = max(dot(normal, vec3(lightDir)), 0.0);
	vec3 diffuse = dir.diffuse * diff * vec3(texture(u_material.diffuse, TexCoord));

	// Specular
	vec3 reflectDir = reflect(vec3(-lightDir), normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	vec3 specular = dir.specular * spec * vec3(texture(u_material.specular, TexCoord));

	return ambient + diffuse + specular;
}