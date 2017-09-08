#ifndef SST_LIGHT_H
#define SST_LIGHT_H

#include <MathSpot.h>

#include "Graphics.h"
#include "Color.h"

namespace mst = mathspot;

namespace sunspot {


class ShaderProgram;


class Light {
public:
	Light(const float r, const float g, const float b);

	inline mst::Vec3 &getPosition() { return mPosition; }
	inline void setPosition(const float x, const float y, const float z) { mPosition.x = x; mPosition.y = y; mPosition.z = z; }
	inline Color &getColor() { return mColor; }

	void update(const ShaderProgram& program) const;

private:
	mst::Vec3 mPosition;
	Color mColor;
};


}


#endif // SST_LIGHT_H
