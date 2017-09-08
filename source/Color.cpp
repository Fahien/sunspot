#include "Color.h"


sunspot::Color::Color()
:	r{ 1.0f }
,	g{ 1.0f }
,	b{ 1.0f }
{}


sunspot::Color::Color(const float rr, const float gg, const float bb)
:	r{ rr }
,	g{ gg }
,	b{ bb }
{}


std::ostream& sunspot::operator<<(std::ostream& os, const Color& c)
{
	return os << "[" << c.r << ", " << c.g << ", " << c.b << "]";
}
