#ifndef SST_LIGHT_H
#define SST_LIGHT_H

#include <MathSpot.h>

#include "Graphics.h"
#include "Color.h"

namespace mst = mathspot;

namespace sunspot {


class ShaderProgram;


class Light
{
public:
	Light(const float r, const float g, const float b);
	Light(Color color) : mAmbient{ color }, mDiffuse{ color }, mSpecular{} {}

	inline Color &GetAmbient()  { return mAmbient;  }
	inline Color &GetDiffuse()  { return mDiffuse;  }
	inline Color &GetSpecular() { return mSpecular; }

	virtual void Update(const ShaderProgram& program) const = 0;

protected:
	Color mAmbient;
	Color mDiffuse;
	Color mSpecular;
};


class DirectionalLight : public Light
{
public:
	DirectionalLight(Color color) : Light{ color } {}

	inline mst::Vec3& GetDirection() { return mDirection; }
	inline void SetDirection(const float x, const float y, const float z) { mDirection.x = x; mDirection.y = y; mDirection.z = z; }

	void Update(const ShaderProgram& program) const override;

private:
	mst::Vec3 mDirection;
};


class PointLight : public Light
{
public:
	PointLight(Color color) : Light{ color } {}

	inline mst::Vec3& GetPosition() { return mPosition; }
	inline void SetPosition(const float x, const float y, const float z) { mPosition.x = x; mPosition.y = y; mPosition.z = z; }

	void Update(const ShaderProgram& program) const override;

private:
	mst::Vec3 mPosition;
};


}


#endif // SST_LIGHT_H
