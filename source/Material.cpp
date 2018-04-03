#include <iostream>
#include <sstream>

#include <Logger.h>

#include "Material.h"
#include "Graphics.h"
#include "ShaderProgram.h"

using namespace std;
using namespace sunspot;
using namespace logspot;


Material::Material()
:	Material("default")
{}


Material::Material(string& n)
:	Material{ n.c_str() }
{}

Material::Material(const char* n)
:	name{ n }
,	ambient {}
,	diffuse {}
,	specular{}
,	hasAmbientMap { false }
,	hasDiffuseMap { false }
,	hasSpecularMap{ false }
{
	Logger::log.info("Material: created %s\n", n); // TODO remove debug log
}


Material::~Material()
{
	if (hasDiffuseMap)
	{
		glDeleteTextures(1, &diffuseMap);
	}

	if (hasSpecularMap)
	{
		glDeleteTextures(1, &specularMap);
	}

	Logger::log.info("Material: destroyed %s\n", name.c_str()); // TODO remove debug log
}


void Material::bind(const ShaderProgram& shader) const
{
	// Bind PBR base colour
	glUniform3f(shader.getLocation("material.color"), color.r, color.g, color.b);
	// Bind PVR metallic factor
	glUniform1f(shader.getLocation("material.metallic"), metallic);
	// Bind PVR roughness factor
	glUniform1f(shader.getLocation("material.roughness"), roughness);
	// Bind PVR ambient occlusion
	glUniform1f(shader.getLocation("material.ambientOcclusion"), ambientOcclusion);
	// Bind ambient color
	glUniform3f(shader.getLocation("material.ambient"),  ambient.r,  ambient.g,  ambient.b);
	// Bind diffuse color
	glUniform3f(shader.getLocation("material.diffuse"),  diffuse.r,  diffuse.g,  diffuse.b);
	// Bind specular color
	glUniform3f(shader.getLocation("material.specular"), specular.r, specular.g, specular.b);
	// Bind shininess
	glUniform1f(shader.getLocation("material.shininess"), shininess);
	// TODO ambient flag and map
	// Bind diffuse flag
	glUniform1i(shader.getLocation("material.hasDiffuseMap"), hasDiffuseMap);
	// Bind diffuse map
	if (hasDiffuseMap)
	{
		glUniform1i(shader.getLocation("material.diffuseMap"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
	}
	// Bind specular flag
	glUniform1i(shader.getLocation("material.hasSpecularMap"), hasSpecularMap);
	// Bind specular map
	if (hasSpecularMap)
	{
		glUniform1i(shader.getLocation("material.specularMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
	}
}


ostream& operator<<(ostream& os, const Material& m)
{
	return os << "Material["      << m.name        << "]\n"
	          << "\tambient["     << m.ambient     << "]\n"
	          << "\tdiffuse["     << m.diffuse     << "]\n"
	          << "\tspecular["    << m.specular    << "]\n"
	          << "\tdiffuseMap["  << m.diffuseMap  << "]\n"
	          << "\tspecularMap[" << m.specularMap << "]";
}
