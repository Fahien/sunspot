#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D headerTexture;
uniform vec2 frameSize;

void main()
{
	vec2 headerCoords = vec2(texCoords.x, (1.0 - texCoords.y) * 540.0f);
	vec4 header = texture(headerTexture, headerCoords);
	color = texture(colorTexture, texCoords);
	color.b = header.r != 1.0 ? header.b : color.b;
}
