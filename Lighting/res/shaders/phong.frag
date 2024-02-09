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

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform sampler2D myTexture;
uniform float time;

void main()
{
	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

	// Specular
	vec3 viewDir = normalize(vec3(0.0) - FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	vec3 emission = ((sin(time) + 1) / 2) * texture(material.emission, TexCoord).rgb;

	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
};