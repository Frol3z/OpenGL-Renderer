#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 Normal;
out vec3 FragPosition;
out vec2 TexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(vPosition, 1.0);
	
	Normal = mat3(transpose(inverse(u_view * u_model))) * vNormal;
	FragPosition = vec3(u_view * u_model * vec4(vPosition, 1.0));
	TexCoord = vTexCoord;
};