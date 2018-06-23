#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoords;
layout (location = 3) in vec4 in_Color;

out vec3 position;
out vec3 normal;
out vec2 texCoords;
out vec4 vertColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 position4 = vec4(in_Position, 1.0);
	position = vec3(model * position4);
	gl_Position = projection * view * model * position4;
	normal = mat3(transpose(inverse(model))) * in_Normal;
	texCoords = in_TexCoords;
	vertColor = in_Color;
}
