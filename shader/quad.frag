#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D screenTexture;
uniform sampler2D maskTexture;

const vec4 empty = vec4(0);

void main()
{
	color = texture(screenTexture, texCoords);
	vec2 maskCoords = vec2(texCoords.x, texCoords.y * 540);
	float mask = texture(maskTexture, maskCoords).r;
	color = mix(color, empty, mask);
}
