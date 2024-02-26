#version 460 core

#define MAX_POINT_LIGHTS 128

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

	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
};

struct DirLight
{
	vec3 direction; // from the light
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	float radius;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material u_material;
uniform DirLight u_dirLight;

uniform int u_pointLightsCount;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

uniform bool u_isTextured;

vec3 CalcDirLight(DirLight dir, Material mat, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 viewDir);

float Attenuate(float dist, float radius);

void main()
{
	vec3 result = vec3(0.0);

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(vec3(0.0) - FragPosition);

	result += CalcDirLight(u_dirLight, u_material, norm, viewDir);

	for(int i = 0; i < u_pointLightsCount; i++)
      result += CalcPointLight(u_pointLights[i], u_material, norm, viewDir);

	// @todo Spot lights

	if(u_isTextured)
	  result += vec3(texture(u_material.emissionMap, TexCoord));

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight dir, Material mat, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-dir.direction);
	vec3 ambient, diffuse, specular;

	if(u_isTextured)
	{
		// Ambient
		ambient = dir.ambient * vec3(texture(u_material.diffuseMap, TexCoord));

		// Diffuse
		float diff = max(dot(normal, vec3(lightDir)), 0.0);
		diffuse = dir.diffuse * diff * vec3(texture(u_material.diffuseMap, TexCoord));

		// Specular
		vec3 reflectDir = reflect(vec3(-lightDir), normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
		specular = dir.specular * spec * vec3(texture(u_material.specularMap, TexCoord));
	} 
	else
	{
		// Ambient
		ambient = dir.ambient * mat.ambient;

		// Diffuse
		float diff = max(dot(normal, vec3(lightDir)), 0.0);
		diffuse = dir.diffuse * diff * mat.diffuse;

		// Specular
		vec3 reflectDir = reflect(vec3(-lightDir), normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
		specular = dir.specular * spec * mat.specular;
	}

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPosition);
	vec3 ambient, diffuse, specular;
	float distance = length(light.position - FragPosition);
	float attenuation = Attenuate(distance, light.radius);

	if(u_isTextured)
	{
		// Ambient
		ambient = light.ambient * vec3(texture(u_material.diffuseMap, TexCoord));

		// Diffuse
		float diff = max(dot(normal, vec3(lightDir)), 0.0);
		diffuse = light.diffuse * diff * vec3(texture(u_material.diffuseMap, TexCoord));

		// Specular
		vec3 reflectDir = reflect(vec3(-lightDir), normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
		specular = light.specular * spec * vec3(texture(u_material.specularMap, TexCoord));
	} 
	else
	{
		// Ambient
		ambient = light.ambient * mat.ambient;

		// Diffuse
		float diff = max(dot(normal, vec3(lightDir)), 0.0);
		diffuse = light.diffuse * diff * mat.diffuse;

		// Specular
		vec3 reflectDir = reflect(vec3(-lightDir), normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
		specular = light.specular * spec * mat.specular;
	}

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

float Attenuate(float dist, float radius)
{
	float attenuation = radius / max(pow(dist, 2), 0.2); // 0.2 = hardcoded radius of the default point light sphere mesh (@todo change)
	float windowingValue = pow( max( pow(1 - (dist / radius), 4), 0 ), 2);
	return windowingValue * attenuation;
}