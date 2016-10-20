#version 330 core

const float one = 1.0;

// 42" constants
const float Zd = 0.467481; // Display pane depth
const float vz = 7.655192; // View distance in coordinate units
const float M = -1960.37; // Linear function multiplier
const float C = 127.5; // Linear function constant

out vec4 color;


// Translates a depth [0,1] into a disparity [0,255]
float disparity(float z)
{
	z = (-z + one) / 2.0;
	return (M * (1 - vz / (z - Zd + vz)) + C) / 255.0;
}


void main()
{
	float d = disparity(gl_FragCoord.z);
	color = vec4(d, d, d, one);
}
