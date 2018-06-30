#ifndef SST_COLOR_H
#define SST_COLOR_H

#include <iostream>


namespace sunspot
{

struct Color
{
	Color();
	Color(const float r, const float g, const float b);

	float r; // Red
	float g; // Green
	float b; // Blue

	friend std::ostream& operator<<(std::ostream& os, const Color& c);
};

std::ostream& operator<<(std::ostream& os, const Color& c);

}

#endif // SST_COLOR_H
