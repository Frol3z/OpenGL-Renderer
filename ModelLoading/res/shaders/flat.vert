#version 460 core

#define MAX_POINT_LIGHTS 128

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out flat vec3 Color;

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

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

vec3 CalcDirLight(DirLight dir, Material mat, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 viewDir);

float Attenuate(float dist, float radius);

void main()
{
	// Compute vertex position
	gl_Position = u_projection * u_view * u_model * vec4(vPosition, 1.0);

	// Gouraud shader
	vec3 result = vec3(0.0);
	vec3 norm = mat3(transpose(inverse(u_view * u_model))) * vNormal;
	vec3 vertPosition = vec3(u_view * u_model * vec4(vPosition, 1.0));
	vec3 viewDir = normalize(vec3(0.0) - vertPosition);

	result += CalcDirLight(u_dirLight, u_material, norm, viewDir);

	for(int i = 0; i < u_pointLightsCount; i++)
      result += CalcPointLight(u_pointLights[i], u_material, norm, viewDir);

	// @todo Spot lights
	Color = result;
};

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

	return ambient + diffuse;
}

vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 viewDir)
{
	vec3 distanceVec = vec3(vec4(light.position, 1.0) - (u_view * u_model * vec4(vPosition, 1.0)));
	vec3 lightDir = normalize(distanceVec);
	vec3 ambient, diffuse, specular;
	float distance = length(distanceVec);
	float attenuation = Attenuate(distance, light.radius);

	// Ambient
	ambient = light.ambient * mat.ambient;

	// Diffuse
	float diff = max(dot(normal, vec3(lightDir)), 0.0);
	diffuse = light.diffuse * diff * mat.diffuse;

	// Specular
	vec3 reflectDir = reflect(vec3(-lightDir), normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	specular = light.specular * spec * mat.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse;
}

float Attenuate(float dist, float radius)
{
	float attenuation = radius / max(pow(dist, 2), 0.2); // 0.2 = hardcoded radius of the default point light sphere mesh (@todo change)
	float windowingValue = pow( max( pow(1 - (dist / radius), 4), 0 ), 2);
	return windowingValue * attenuation;
}