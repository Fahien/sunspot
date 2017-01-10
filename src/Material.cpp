#include <iostream>
#include <sstream>

#include "Material.h"
#include "Graphics.h"
#include "ShaderProgram.h"

using namespace sunspot;


Material::Material()
	: Material("default")
{}


Material::Material(const char *n)
	: name{ n }
	, ambient{}
	, diffuse{}
	, specular{}
{}


Material::Material(std::string &n)
	: name{ n }
	, ambient{}
	, diffuse{}
	, specular{}
{}


Material::~Material()
{
	glDeleteTextures(1, &diffuseMap);
	glDeleteTextures(1, &specularMap);
}


void Material::bind(const ShaderProgram *shader) const {
	// Bind ambient color
	glUniform3f(shader->getLocation("material.ambient"), ambient.r, ambient.g, ambient.b);
	// Bind diffuse color
	glUniform3f(shader->getLocation("material.diffuse"), diffuse.r, diffuse.g, diffuse.b);
	// Bind specular color
	glUniform3f(shader->getLocation("material.specular"), specular.r, specular.g, specular.b);
	// Bind shininess
	glUniform1f(shader->getLocation("material.shininess"), shininess);
	// Bind diffuse map
	glUniform1i(shader->getLocation("material.diffuseMap"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	// Bind specular map
	glUniform1i(shader->getLocation("material.specularMap"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
}


std::ostream& sunspot::operator<<(std::ostream& os, const Material& m)
{
	return os << "[" << m.ambient.r << ", " << m.ambient.g << ", " << m.ambient.b << "]";
}
