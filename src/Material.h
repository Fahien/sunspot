#ifndef SST_MATERIAL_H
#define SST_MATERIAL_H

#include "Graphics.h"
#include "Color.h"

struct Material {
	GLuint diffuse;
	GLuint specular;
	float shininess = 32.0f;
};

#endif // SST_MATERIAL_H

