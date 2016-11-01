#include <cmath>

#include "Camera.h"


Camera::Camera(const float fov, const float aspectRatio, const float near, const float far)
	: view_ {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, -3, 1}}
	, projection_{ 0 }
{
	float cotfov{ 1.0f / tan(fov * pi / 180.0f / 2.0f) };
	projection_[0][0] = cotfov / aspectRatio;
	projection_[1][1] = cotfov;
	projection_[2][2] = (near + far) / (near - far);
	projection_[3][2] = 2 * near * far / (near - far);
	projection_[2][3] = -1;
}