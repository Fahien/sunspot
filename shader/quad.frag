#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D screenTexture;
uniform sampler2D maskTexture;
uniform sampler2D headerTexture;
uniform vec2 frameSize;

const vec2 headerSize = vec2(960.0f, 2.0f);
const vec4 empty = vec4(0.0f);

void main()
{
	vec2 headerCoords = vec2(texCoords.x * frameSize.x / headerSize.x,
		texCoords.y * frameSize.y / headerSize.y - frameSize.y / headerSize.y + 1.0f);
	vec4 header = texture(headerTexture, headerCoords);
	color = texture(screenTexture, texCoords);
	color.b = header.r != 1.0 ? header.b : color.b;
	vec2 maskCoords = vec2(texCoords.x, texCoords.y * frameSize.y);
	float mask = texture(maskTexture, maskCoords).r;
	color = mix(color, empty, mask);
}

