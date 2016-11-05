#include <cmath>

#include "Camera.h"


Camera::Camera(const float fov, const float aspectRatio, const float near, const float far)
	: view_{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -3.0f, 1.0f }
	, projection_{ }
{
	float cotfov{ 1.0f / tan(fov * pi / 180.0f / 2.0f) };
	projection_[0] = cotfov / aspectRatio;
	projection_[5] = cotfov;
	projection_[10] = (near + far) / (near - far);
	projection_[14] = 2 * near * far / (near - far);
	projection_[11] = -1;
}