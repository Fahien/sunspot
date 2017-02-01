#version 330 core

const float one = 1.0f;
const vec4 empty = vec4(0.0f);

// 42" constants
const float Zd = 0.467481; // Display pane depth
const float vz = 7.655192; // View distance in coordinate units
const float M = -1960.37; // Linear function multiplier
const float C = 127.5; // Linear function constant

uniform sampler2D depthTexture;
uniform vec2 frameSize;

in vec2 texCoords;

out vec4 color;

// Translates a depth [0,1] into a disparity [0,255]
float disparity(float z)
{
	z = (z + one) / 2.0f;
	return (M * (1 - vz / (z - Zd + vz)) + C);
}

void main()
{
	color = texture(depthTexture, texCoords);
	color.r = color.g = color.b = disparity(color.r);
}
