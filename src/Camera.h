#ifndef SST_CAMERA_H
#define SST_CAMERA_H

#include "ShaderProgram.h"
#include "Math.h"

class Camera
{
  public:
	const float pi{ 3.14159265f };
	Camera(const float fov, const float aspectRatio, const float near, const float far);

	void update(const ShaderProgram& program) const
	{
		GLuint transformLoc{ program.getLocation("view") };
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, view_.matrix_);
		transformLoc = program.getLocation("projection");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, projection_.matrix_);
	};
  private:
	math::Mat4 view_;
	math::Mat4 projection_;
};

#endif // SST_CAMERA_H