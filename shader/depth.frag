#version 330 core

const float one = 1.0;
const vec4 empty = vec4(0);

// 42" constants
const float Zd = 0.467481; // Display pane depth
const float vz = 7.655192; // View distance in coordinate units
const float M = -1960.37; // Linear function multiplier
const float C = 127.5; // Linear function constant

uniform sampler2D depthTexture;
uniform sampler2D maskTexture;
uniform vec2 frameSize;

in vec2 texCoords;

out vec4 color;

// Translates a depth [0,1] into a disparity [0,255]
float disparity(float z)
{
	z = (z + one) / 2.0;
	return (M * (1 - vz / (z - Zd + vz)) + C) / 24.0;
}

void main()
{
	color = texture(depthTexture, texCoords);
	color.r = color.g = color.b = disparity(color.r);
	
	vec2 maskCoords = vec2(texCoords.x, texCoords.y * frameSize.y);
	float mask = texture(maskTexture, maskCoords).r;
	color = mix(color, empty, mask);
}
