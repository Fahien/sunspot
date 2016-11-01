#ifndef SST_CAMERA_H
#define SST_CAMERA_H

#include "ShaderProgram.h"

class Camera
{
  public:
	const float pi{ 3.14159265f };
	Camera(const float fov, const float aspectRatio, const float near, const float far);

	void update(const ShaderProgram& program) const
	{
		GLuint transformLoc{ program.getLocation("view") };
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, view_[0]);
		transformLoc = program.getLocation("projection");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, projection_[0]);
	};
  private:
	float view_[4][4];
	float projection_[4][4];
};

#endif // SST_CAMERA_H