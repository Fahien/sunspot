#version 330 core

out vec4 o_color;

struct Vertex
{
	bool hasColor;
};

uniform Vertex vertex;

void main()
{
	o_color = vec4(1.0, 1.0, 1.0, 1.0);
}
