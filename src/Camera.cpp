#include <cmath>

#include "Camera.h"
#include "Graphics.h"
#include "ShaderProgram.h"


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


void Camera::update(const ShaderProgram *program) const
{
	GLuint transformLoc{ program->getLocation("view") };
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, view_.matrix_);
	transformLoc = program->getLocation("projection");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, projection_.matrix_);
};