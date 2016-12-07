#include "Light.h"
#include "ShaderProgram.h"

using namespace sunspot;


Light::Light(const float r, const float g, const float b)
	: position_{}
	, color_{ r, g, b }
{}


void Light::update(const ShaderProgram *program) const
{
	GLuint location{ program->getLocation("light.position") };
	glUniform3fv(location, 1, &position_.x);

	location = program->getLocation("light.ambient");
	glUniform3fv(location, 1, &color_.r);

	location = program->getLocation("light.diffuse");
	glUniform3fv(location, 1, &color_.r);

	location = program->getLocation("light.specular");
	glUniform3fv(location, 1, &color_.r);
}

