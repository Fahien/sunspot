#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform sampler2D maskTexture;
uniform sampler2D headerTexture;

const vec4 empty = vec4(0);

void main()
{
	vec4 header = texture(headerTexture, texCoords);
	color = texture(screenTexture, texCoords);
	color.b = header.r != 1.0 ? header.b : color.b;
	vec2 maskCoords = vec2(texCoords.x, texCoords.y * 540);
	float mask = texture(maskTexture, maskCoords).r;
	color = mix(color, empty, mask);
}
