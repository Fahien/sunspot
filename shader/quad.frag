#version 330 core

const float one = 1.0f;
const float zero = 0.0f;

in vec2 texCoords;

out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D headerTexture;
uniform vec2 frameSize;

void main()
{
	vec2 headerCoords = vec2(texCoords.x, (one - texCoords.y) * frameSize.y);
	vec4 header = texture(headerTexture, headerCoords);
	color = texture(colorTexture, texCoords);
	if(header.r == zero)
		color.b = header.b;
}
