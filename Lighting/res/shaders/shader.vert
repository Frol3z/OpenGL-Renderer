#version 460 core

layout (location = 0) in vec3 vPosition;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 uColor;

void main()
{
	color = uColor;
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
};