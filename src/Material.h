#ifndef SST_MATERIAL_H
#define SST_MATERIAL_H

#include "Color.h"

struct Material {
	Color ambient;
	Color diffuse;
	Color specular;
	float shininess = 32.0f;
};

#endif SST_MATERIAL_H