#ifndef SST_LIGHT_H
#define SST_LIGHT_H

#include "Graphics.h"
#include "Color.h"

class ShaderProgram;


class Light {
  public:
	Light(const float r, const float g, const float b);

	inline Color &getColor() { return color_; }

	void update(const ShaderProgram *program) const;

  private:
	Color color_;
};

#endif