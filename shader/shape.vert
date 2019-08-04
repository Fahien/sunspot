#version 330 core

layout (location = 0) in vec3 i_position;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 position4 = vec4(i_position, 1.0);
	gl_Position = projection * view * model * position4;
}
