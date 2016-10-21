#version 330 core

const float one = 1.0;

in vec3 theColor;

out vec4 color;

void main()
{
	color = vec4(theColor, one);
}
