#include <cmath>

#include "Camera.h"
#include "Graphics.h"
#include "ShaderProgram.h"

using namespace sunspot;
namespace mst = mathspot;


Camera::Camera(const float fov, const float aspectRatio, const float near, const float far)
	: mRotation{ mst::Mat4::identity }
	, mTranslation{ mst::Mat4::identity }
	, mView{ mst::Mat4::identity }
	, mProjection{}
	, mPitch{ 0.0f }
	, mYaw{ -mst::kPi / 2.0f }
	, mPosition{ 0.0f, 0.0f, -1.0f }
	, mDirection{ 0.0f, 0.0f, 1.0f }
	, mUp{ 0.0f, 1.0f, 0.0f }
	, mRight{ 1.0f, 0.0f, 0.0f }
	, mWorldUp{ 0.0f, 1.0f, 0.0f }
	, mVelocity{}
	, mVelocityFactor{ 8.0f }
{
	float cotfov{ 1.0f / static_cast<float>(std::tan(fov)) };
	mProjection[0] = cotfov / aspectRatio;
	mProjection[5] = cotfov;
	mProjection[10] = (near + far) / (near - far);
	mProjection[14] = 2 * near * far / (near - far);
	mProjection[11] = -1;
	updateVectors();
}


void Camera::setPosition(const float x, const float y, const float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
}


void Camera::LookAt(const mst::Vec3& p)
{
	LookAt(p.x, p.y, p.z);
}


void Camera::LookAt(const float x, const float y, const float z)
{
	mDirection.x = x - mPosition.x;
	mDirection.y = y - mPosition.y;
	mDirection.z = z - mPosition.z;
	mDirection.Normalize();
	updateVectors();
}


void Camera::updateVectors()
{
	//float cospitch{ static_cast<float>(std::cos(mPitch)) };
	//float sinpitch{ static_cast<float>(std::sin(mPitch)) };
	//float cosyaw{ static_cast<float>(std::cos(mYaw)) };
	//float sinyaw{ static_cast<float>(std::sin(mYaw)) };
	//mDirection.x = cospitch * cosyaw;
	//mDirection.y = sinpitch;
	//mDirection.z = cospitch * sinyaw;
	//mDirection.normalize();
	if (true)
	{
		mRight.x = 1.0f;
		mRight.y = 0.0f;
		mRight.z = 0.0f;
	}
	else
	{
		mRight = mst::Vec3::Cross(mDirection, mWorldUp);
		mRight.Normalize();
	}
	mUp = mst::Vec3::Cross(mRight, mDirection);
	mUp.Normalize();
}


void Camera::updateView()
{
	mRotation[0] = mRight.x;
	mRotation[1] = mUp.x;
	mRotation[2] = mDirection.x;
	mRotation[4] = mRight.y;
	mRotation[5] = mUp.y;
	mRotation[6] = mDirection.y;
	mRotation[8] = mRight.z;
	mRotation[9] = mUp.z;
	mRotation[10] = mDirection.z;
	mTranslation[12] = mPosition.x;
	mTranslation[13] = mPosition.y;
	mTranslation[14] = mPosition.z;
	mView = mRotation * mTranslation;
}


void Camera::update(const float deltaTime, const ShaderProgram& program)
{
	if (mVelocity.x != 0.0f)
	{
		mPosition.x += mRight.x * mVelocity.x * mVelocityFactor * deltaTime;
		mPosition.y += mRight.y * mVelocity.x * mVelocityFactor * deltaTime;
		mPosition.z += mRight.z * mVelocity.x * mVelocityFactor * deltaTime;
	}

	if (mVelocity.y != 0.0f)
	{
		mPosition.x += mUp.x * mVelocity.y * mVelocityFactor * deltaTime;
		mPosition.y += mUp.y * mVelocity.y * mVelocityFactor * deltaTime;
		mPosition.z += mUp.z * mVelocity.y * mVelocityFactor * deltaTime;
	}

	if (mVelocity.z != 0.0f)
	{
		mPosition.x += mDirection.x * mVelocity.z * mVelocityFactor * deltaTime;
		mPosition.y += mDirection.y * mVelocity.z * mVelocityFactor * deltaTime;
		mPosition.z += mDirection.z * mVelocity.z * mVelocityFactor * deltaTime;
	}

	updateView();

	GLuint location{ program.getLocation("view") };
	glUniformMatrix4fv(location, 1, GL_FALSE, mView.matrix);

	location = program.getLocation("projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, mProjection.matrix);

	location = program.getLocation("camera.position");
	glUniform3fv(location, 1, &mPosition.x);
};
