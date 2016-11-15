#ifndef SST_LIGHT_H
#define SST_LIGHT_H

#include "Graphics.h"
#include "Math.h"
#include "Color.h"

class ShaderProgram;

using namespace math;

class Light {
  public:
	Light(const float r, const float g, const float b);

	inline Vec3 &getPosition() { return position_; }
	inline void setPosition(const float x, const float y, const float z) { position_.x = x; position_.y = y; position_.z = z; }
	inline Color &getColor() { return color_; }

	void update(const ShaderProgram *program) const;

  private:
	Vec3 position_;
	Color color_;
};

#endif