#include "Light.h"
#include "ShaderProgram.h"

using namespace sunspot;


Light::Light(const float r, const float g, const float b)
	: mAmbient { r, g, b }
	, mDiffuse { r, g, b }
	, mSpecular{ r, g, b }
{}


void DirectionalLight::Update(const ShaderProgram& program) const
{
	GLuint location{ program.getLocation("directionalLightActive") };
	glUniform1i(location, true);

	location = program.getLocation("dLight.direction");
	glUniform3fv(location, 1, &mDirection.x);

	location = program.getLocation("dLight.ambient");
	glUniform3fv(location, 1, &mAmbient.r);

	location = program.getLocation("dLight.diffuse");
	glUniform3fv(location, 1, &mDiffuse.r);

	location = program.getLocation("dLight.specular");
	glUniform3fv(location, 1, &mSpecular.r);
}

/*
PointLight::PointLight(const float r, const float g, const float b)
	: mPosition{}
	, mColor{ r, g, b }
{}
*/

void PointLight::Update(const ShaderProgram& program) const
{
	GLuint location{ program.getLocation("pointLightActive") };
	glUniform1i(location, true);

	location = program.getLocation("pLight.position");
	glUniform3fv(location, 1, &mPosition.x);

	location = program.getLocation("pLight.ambient");
	glUniform3fv(location, 1, &mAmbient.r);

	location = program.getLocation("pLight.diffuse");
	glUniform3fv(location, 1, &mDiffuse.r);

	location = program.getLocation("pLight.specular");
	glUniform3fv(location, 1, &mSpecular.r);
}
