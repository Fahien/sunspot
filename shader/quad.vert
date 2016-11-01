#version 330 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_TexCoords;

out vec2 texCoords;

void main()
{
	texCoords = in_TexCoords;
	gl_Position = vec4(in_Position.x, in_Position.y, 0.0f, 1.0f);
}