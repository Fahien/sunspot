#include <iostream>
#include <sstream>

#include "Material.h"
#include "Logger.h"
#include "Graphics.h"
#include "ShaderProgram.h"

namespace sst = sunspot;


sst::Material::Material()
:	sst::Material("default")
{}


sst::Material::Material(std::string& n)
:	sst::Material{ n.c_str() }
{}


sst::Material::Material(const char* n)
:	name{ n }
,	ambient {}
,	diffuse {}
,	specular{}
,	hasAmbientMap { false }
,	hasDiffuseMap { false }
,	hasSpecularMap{ false }
{
	sst::Logger::log.info("Material: created %s\n", n); // TODO remove debug log
}


sst::Material::~Material()
{
	if (hasDiffuseMap)
		glDeleteTextures(1, &diffuseMap);
	if (hasSpecularMap)
		glDeleteTextures(1, &specularMap);

	sst::Logger::log.info("Material: destroyed %s\n", name.c_str()); // TODO remove debug log
}


void sst::Material::bind(const sst::ShaderProgram* shader) const
{
	// Bind ambient color
	glUniform3f(shader->getLocation("material.ambient"),  ambient.r,  ambient.g,  ambient.b);
	// Bind diffuse color
	glUniform3f(shader->getLocation("material.diffuse"),  diffuse.r,  diffuse.g,  diffuse.b);
	// Bind specular color
	glUniform3f(shader->getLocation("material.specular"), specular.r, specular.g, specular.b);
	// Bind shininess
	glUniform1f(shader->getLocation("material.shininess"), shininess);
	// TODO ambient flag and map
	// Bind diffuse flag
	glUniform1i(shader->getLocation("material.hasDiffuseMap"), hasDiffuseMap);
	// Bind diffuse map
	if (hasDiffuseMap)
	{
		glUniform1i(shader->getLocation("material.diffuseMap"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
	}
	// Bind specular flag
	glUniform1i(shader->getLocation("material.hasSpecularMap"), hasSpecularMap);
	// Bind specular map
	if (hasSpecularMap)
	{
		glUniform1i(shader->getLocation("material.specularMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
	}
}


std::ostream& sst::operator<<(std::ostream& os, const Material& m)
{
	return os << "Material["      << m.name        << "]\n"
	          << "\tambient["     << m.ambient     << "]\n"
	          << "\tdiffuse["     << m.diffuse     << "]\n"
	          << "\tspecular["    << m.specular    << "]\n"
	          << "\tdiffuseMap["  << m.diffuseMap  << "]\n"
	          << "\tspecularMap[" << m.specularMap << "]";
}
