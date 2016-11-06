#ifndef SST_CAMERA_H
#define SST_CAMERA_H

#include "Math.h"

class ShaderProgram;

class Camera
{
  public:
	const float pi{ 3.14159265f };
	Camera(const float fov, const float aspectRatio, const float near, const float far);

	void update(const ShaderProgram *program) const;

  private:
	math::Mat4 view_;
	math::Mat4 projection_;
};

#endif // SST_CAMERA_H