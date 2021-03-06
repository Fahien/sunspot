#ifndef SST_CAMERA_H
#define SST_CAMERA_H

#include <MathSpot.h>

namespace mst = mathspot;

namespace sunspot {

class ShaderProgram;

class Camera {
public:
	Camera(const float fov, const float aspectRatio, const float near, const float far);

	void setPosition(const float x, const float y, const float z);

	inline float& getPitch() { return mPitch; }
	inline float& getYaw()   { return mYaw;   }

	inline void setPitch(const float& pitch) { mPitch = pitch; }
	inline void setYaw(const float& yaw)     { mYaw = yaw; }

	void updateVectors();
	void updateView();

	void update(const float deltaTime, const ShaderProgram& program);

	inline void setVelocityX(const float x) { mVelocity.x = x; }
	inline void setVelocityY(const float y) { mVelocity.y = y; }
	inline void setVelocityZ(const float z) { mVelocity.z = z; }

private:
	mst::Mat4 mRotation;
	mst::Mat4 mTranslation;
	mst::Mat4 mView;
	mst::Mat4 mProjection;

	float mPitch;
	float mYaw;

	mst::Vec3 mPosition;
	mst::Vec3 mDirection;
	mst::Vec3 mUp;
	mst::Vec3 mRight;

	mst::Vec3 mWorldUp;

	mst::Vec3 mVelocity;
	float mVelocityFactor;
};

}

#endif // SST_CAMERA_H
