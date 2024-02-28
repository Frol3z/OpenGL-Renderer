#version 460 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPosition;

out vec4 FragColor;

uniform vec3 u_Color;

void main()
{
	FragColor = vec4(u_Color, 1.0);
};