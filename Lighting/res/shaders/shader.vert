#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPosition;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	Normal = mat3(transpose(inverse(view * model))) * vNormal;
	TexCoord = vTexCoord;
	FragPosition = vec3(view * model * vec4(vPosition, 1.0));
};