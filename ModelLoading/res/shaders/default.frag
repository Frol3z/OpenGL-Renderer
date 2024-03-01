#version 460 core

in vec2 TexCoord;

out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_diffuse4;

	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	sampler2D texture_specular4;

	sampler2D texture_emissive1;
	sampler2D texture_emissive2;
};

uniform Material u_Material;

void main()
{
	vec3 result = vec3(0.0);

	result += vec3(texture(u_Material.texture_diffuse1, TexCoord));

	FragColor = vec4(result, 1.0);
}