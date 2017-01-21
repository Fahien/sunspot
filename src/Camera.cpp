#include <cmath>

#include "Camera.h"
#include "Graphics.h"
#include "ShaderProgram.h"

using namespace sunspot;


Camera::Camera(const float fov, const float aspectRatio, const float near, const float far)
	: rotation_{ math::Mat4::identity }
	, translation_{ math::Mat4::identity }
	, view_{ math::Mat4::identity }
	, projection_{}
	, pitch_{ 0.0f }
	, yaw_{ -math::pi / 2.0f }
	, position_{ 0.0f, 0.0f, -1.0f }
	, direction_{ 0.0f, 0.0f, 1.0f }
	, up_{ 0.0f, 1.0f, 0.0f }
	, right_{ 1.0f, 0.0f, 0.0f }
	, worldUp_{ 0.0f, 1.0f, 0.0f }
	, velocity_{}
	, velocityFactor_{ 8.0f }
{
	float cotfov{ 1.0f / static_cast<float>(std::tan(fov * math::pi / 360.0f)) };
	projection_[0] = cotfov / aspectRatio;
	projection_[5] = cotfov;
	projection_[10] = (near + far) / (near - far);
	projection_[14] = 2 * near * far / (near - far);
	projection_[11] = -1;
	updateVectors();
}


void Camera::updateVectors()
{
	float cospitch{ static_cast<float>(std::cos(pitch_)) };
	float sinpitch{ static_cast<float>(std::sin(pitch_)) };
	float cosyaw{ static_cast<float>(std::cos(yaw_)) };
	float sinyaw{ static_cast<float>(std::sin(yaw_)) };
	direction_.x = cospitch * cosyaw;
	direction_.y = sinpitch;
	direction_.z = cospitch * sinyaw;
	direction_.normalize();
	right_ = math::Vec3::cross(direction_, worldUp_);
	right_.normalize();
	up_ = math::Vec3::cross(right_, direction_);
	up_.normalize();
}


void Camera::updateView()
{
	rotation_[0] = right_.x;
	rotation_[1] = up_.x;
	rotation_[2] = direction_.x;
	rotation_[4] = right_.y;
	rotation_[5] = up_.y;
	rotation_[6] = direction_.y;
	rotation_[8] = right_.z;
	rotation_[9] = up_.z;
	rotation_[10] = direction_.z;
	translation_[12] = -position_.x;
	translation_[13] = -position_.y;
	translation_[14] = -position_.z;
	view_ = rotation_ * translation_;
}


void Camera::update(const float deltaTime, const ShaderProgram *program)
{
	if (velocity_.x != 0.0f) {
		position_.x += right_.x * velocity_.x * velocityFactor_ * deltaTime;
		position_.y += right_.y * velocity_.x * velocityFactor_ * deltaTime;
		position_.z += right_.z * velocity_.x * velocityFactor_ * deltaTime;
	}
	if (velocity_.y != 0.0f) {
		position_.x += up_.x * velocity_.y * velocityFactor_ * deltaTime;
		position_.y += up_.y * velocity_.y * velocityFactor_ * deltaTime;
		position_.z += up_.z * velocity_.y * velocityFactor_ * deltaTime;
	}
	if (velocity_.z != 0.0f) {
		position_.x += direction_.x * velocity_.z * velocityFactor_ * deltaTime;
		position_.y += direction_.y * velocity_.z * velocityFactor_ * deltaTime;
		position_.z += direction_.z * velocity_.z * velocityFactor_ * deltaTime;
	}
	updateView();

	GLuint location{ program->getLocation("view") };
	glUniformMatrix4fv(location, 1, GL_FALSE, view_.matrix);
	location = program->getLocation("projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, projection_.matrix);
	location = program->getLocation("camera.position");
	glUniform3fv(location, 1, &position_.x);
};
