#ifndef SST_MATERIAL_H
#define SST_MATERIAL_H

#include <fstream>
#include "Graphics.h"
#include "Color.h"

class ShaderProgram;


namespace sunspot {

struct Material {
  public:
	Material();
	Material(const char *n);
	Material(std::string &n);
	~Material();

	void bind(const ShaderProgram *shader) const;

	friend std::ifstream &operator>>(std::ifstream &is, Material &mtl);
	friend std::ostream &operator<<(std::ostream &os, const Material &mtl);

	std::string name;
	Color ambient;
	Color diffuse;
	Color specular;
	float shininess = 32.0f;
	GLuint ambientMap;
	GLuint diffuseMap;
	GLuint specularMap;
};

std::ostream &operator<<(std::ostream &os, const Material &mtl);

}


#endif // SST_MATERIAL_H

