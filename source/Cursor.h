#ifndef SST_INPUT_H
#define SST_INPUT_H

#include <MathSpot.h>

namespace mst = mathspot;


struct GLFWwindow;

class Cursor
{
public:
	Cursor();

	inline mst::Vec2 &getPosition() { return mPosition; }
	void setPosition(const float x, const float y);

	inline mst::Vec2 &getPreviousPosition() { return mPreviousPosition; }
	inline mst::Vec2 &getOffset() { return mOffset; }

	inline float getSensitivity() { return mSensitivity; }
	inline void  setSensitivity(const float sensitivity) { mSensitivity = sensitivity; }

private:
	mst::Vec2 mPosition;
	mst::Vec2 mPreviousPosition;
	mst::Vec2 mOffset;
	bool mFirstCursor;
	float mSensitivity;
};


#endif // SST_INPUT_H
