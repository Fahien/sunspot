#ifndef SST_MATERIAL_H
#define SST_MATERIAL_H

#include <fstream>

#include "Graphics.h"
#include "Color.h"


namespace sunspot
{

class ShaderProgram;

struct Material
{
public:
	Material();
	Material(const char* n);
	Material(std::string& n);
	~Material();

	/// Bind the material to the shader
	void bind(const ShaderProgram& shader) const;

	friend std::ostream& operator<<(std::ostream& os, const Material& mtl);

	std::string name;

	Color color;
	float metallic;
	float roughness;
	float ambientOcclusion;

	Color ambient;
	Color diffuse;
	Color specular;
	float shininess = 32.0f;

	bool hasAmbientMap;
	GLuint ambientMap;

	bool hasDiffuseMap;
	GLuint diffuseMap;

	bool hasSpecularMap;
	GLuint specularMap;
};

}

#endif // SST_MATERIAL_H
