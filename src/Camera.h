#ifndef SST_CAMERA_H
#define SST_CAMERA_H

#include "Math.h"

class ShaderProgram;

class Camera {
  public:
	const float pi{ 3.14159265f };
	Camera(const float fov, const float aspectRatio, const float near, const float far);

	void update(const float deltaTime, const ShaderProgram *program);

	inline void setVelocityX(const float velocityX) { velocityX_ = velocityX; }

	math::Mat4 view;
  private:
	math::Mat4 projection_;
	float velocityX_;
};

#endif // SST_CAMERA_H