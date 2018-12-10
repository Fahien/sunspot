#include <iostream>
#include <sstream>

#include <logspot/Logger.h>

#include "sunspot/system/graphic/Material.h"
#include "sunspot/system/graphic/Shader.h"
#include "sunspot/system/graphic/Texture.h"

using namespace std;
namespace lst = logspot;


namespace sunspot::graphic
{


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
	lst::Logger::log.Info("Material: created %s", n); // TODO remove debug log
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

	lst::Logger::log.Info("Material: destroyed %s", name.c_str()); // TODO remove debug log
}


void Material::bind(const shader::Program& shader) const
{
	// Bind PBR base colour
	glUniform3f(shader.GetLocation("material.color"), color.r, color.g, color.b);

	// Bind PBR base color texture
	glUniform1i(shader.GetLocation("material.hasColorTexture"), colorTexture != nullptr);
	if (colorTexture)
	{
		glUniform1i(shader.GetLocation("material.colorTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexture->getId());
	}

	// Bind PBR metallic factor
	glUniform1f(shader.GetLocation("material.metallic"), metallic);

	// Bind PBR roughness factor
	glUniform1f(shader.GetLocation("material.roughness"), roughness);

	// Bind PBR ambient occlusion
	glUniform1f(shader.GetLocation("material.ambientOcclusion"), ambientOcclusion);

	// TODO deprecate
	// Bind ambient color
	glUniform3f(shader.GetLocation("material.ambient"),  ambient.r,  ambient.g,  ambient.b);
	// Bind diffuse color
	glUniform3f(shader.GetLocation("material.diffuse"),  diffuse.r,  diffuse.g,  diffuse.b);
	// Bind specular color
	glUniform3f(shader.GetLocation("material.specular"), specular.r, specular.g, specular.b);
	// Bind shininess
	glUniform1f(shader.GetLocation("material.shininess"), shininess);
	// TODO ambient flag and map
	// Bind diffuse flag
	glUniform1i(shader.GetLocation("material.hasDiffuseMap"), hasDiffuseMap);
	// Bind diffuse map
	if (hasDiffuseMap)
	{
		glUniform1i(shader.GetLocation("material.diffuseMap"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
	}
	// Bind specular flag
	glUniform1i(shader.GetLocation("material.hasSpecularMap"), hasSpecularMap);
	// Bind specular map
	if (hasSpecularMap)
	{
		glUniform1i(shader.GetLocation("material.specularMap"), 1);
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


} // namespace sunspot::graphic
