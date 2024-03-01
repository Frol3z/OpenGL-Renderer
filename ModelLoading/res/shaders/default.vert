#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec2 TexCoord;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjectionMat;

void main()
{
	gl_Position = u_ProjectionMat * u_ViewMat * u_ModelMat * vec4(vPosition, 1.0);
	TexCoord = vTexCoord;
};