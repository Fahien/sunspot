#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoords;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	texCoords = in_TexCoords;
	gl_Position = projection * view * model * vec4(in_Position, 1.0);
}