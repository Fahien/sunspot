#include "Color.h"


Color::Color()
	: r{ 1.0f }
	, g{ 1.0f }
	, b{ 1.0f }
{}


Color::Color(const float rr, const float gg, const float bb)
	: r{ rr }
	, g{ gg }
	, b{ bb }
{}