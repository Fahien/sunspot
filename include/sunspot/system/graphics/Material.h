#pragma once

#include <fstream>

#include "sunspot/system/graphics/Shader.h"
#include "Color.h"


namespace sunspot::graphics
{

class Texture;

struct Material
{
  public:
	Material();
	Material( const char* n );
	Material( std::string& n );
	~Material();

	/// Bind the material to the shader
	void bind( const shader::Program& shader ) const;

	friend std::ostream& operator<<( std::ostream& os, const Material& mtl );

	std::string name;

	Color color;
	float metallic;
	float roughness;
	float ambientOcclusion;

	Texture* colorTexture = nullptr;

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


} // namespace sunspot::graphics
