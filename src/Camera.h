#ifndef SST_CAMERA_H
#define SST_CAMERA_H

#include "Math.h"

class ShaderProgram;

class Camera {
  public:
	Camera(const float fov, const float aspectRatio, const float near, const float far);

	inline float &getPitch() { return pitch_; }
	inline void setPitch(const float &pitch) { pitch_ = pitch; }

	inline float &getYaw() { return yaw_; }
	inline void setYaw(const float &yaw) { yaw_ = yaw; }

	void updateVectors();
	void updateView();

	void update(const float deltaTime, const ShaderProgram *program);

	inline void setVelocityX(const float x) { velocity_.x = x; }
	inline void setVelocityY(const float y) { velocity_.y = y; }
	inline void setVelocityZ(const float z) { velocity_.z = z; }

  private:
	math::Mat4 rotation_;
	math::Mat4 translation_;
	math::Mat4 view_;
	math::Mat4 projection_;

	float pitch_;
	float yaw_;

	math::Vec3 position_;
	math::Vec3 direction_;
	math::Vec3 up_;
	math::Vec3 right_;

	math::Vec3 worldUp_;

	math::Vec3 velocity_;
	float velocityFactor_;
};

#endif // SST_CAMERA_H
