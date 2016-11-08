#ifndef SST_INPUT_H
#define SST_INPUT_H

#include "Math.h"

struct GLFWwindow;

class Cursor {
public:
	Cursor();

	inline math::Vec2 &getPosition() { return position_; }
	void setPosition(const float x, const float y);

	inline math::Vec2 &getPreviousPosition() { return previousPosition_; }
	inline math::Vec2 &getOffset() { return offset_; }

	inline float getSensitivity() { return sensitivity_; }
	inline void setSensitivity(const float sensitivity) { sensitivity_ = sensitivity; }

private:
	math::Vec2 position_;
	math::Vec2 previousPosition_;
	math::Vec2 offset_;
	bool firstCursor_;
	float sensitivity_;
};

#endif SST_INPUT_H