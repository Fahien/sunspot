#version 330 core

const float one = 1.0;

in vec2 texCoords;

out vec4 color;

uniform sampler2D theTexture;

void main()
{
	color = texture(theTexture, texCoords);
}