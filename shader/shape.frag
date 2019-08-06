#version 330 core

out vec4 o_color;

struct Vertex
{
	bool hasColor;
};

uniform Vertex vertex;

void main()
{
	o_color = vec4(.5, .5, .5, 0.5);
}
