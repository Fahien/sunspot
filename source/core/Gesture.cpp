#include <Gesture.h>

namespace sst = sunspot;


sst::Gesture::Gesture()
:	mSensibility{ 1.0f }
{}


sst::TapGesture::TapGesture()
: mX{ 0.0f }
, mY{ 0.0f }
{}


void sst::TapGesture::Check()
{
	
}
