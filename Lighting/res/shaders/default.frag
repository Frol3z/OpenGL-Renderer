#version 460 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPosition;

out vec4 FragColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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

vec3 CalcDirLight(DirLight dir, Material mat, vec3 normal, vec3 viewDir);

void main()
{
	vec3 result = vec3(0.0);

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(vec3(0.0) - FragPosition);

	result += CalcDirLight(u_dirLight, u_material, norm, viewDir);

	// @todo Point lights
	// @todo Spot lights

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight dir, Material mat, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-dir.direction);

	// Ambient
	vec3 ambient = dir.ambient * mat.ambient;

	// Diffuse
	float diff = max(dot(normal, vec3(lightDir)), 0.0);
	vec3 diffuse = dir.diffuse * diff * mat.diffuse;

	// Specular
	vec3 reflectDir = reflect(vec3(-lightDir), normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	vec3 specular = dir.specular * spec * mat.specular;

	return ambient + diffuse + specular;
}