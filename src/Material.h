#ifndef SST_MATERIAL_H
#define SST_MATERIAL_H

#include <istream>
#include "Graphics.h"
#include "Color.h"

class ShaderProgram;


namespace sunspot {

struct Material {
  public:
	~Material();

	void bind(const ShaderProgram *shader) const;

	Color ambient;
	Color diffuse;
	Color specular;
	float shininess = 32.0f;
	GLuint ambientMap;
	GLuint diffuseMap;
	GLuint specularMap;
};

}


std::ostream& operator<<(std::ostream& os, const sunspot::Material& m);


#endif // SST_MATERIAL_H

