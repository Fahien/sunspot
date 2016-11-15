#include "Light.h"
#include "ShaderProgram.h"


Light::Light(const float r, const float g, const float b)
	: position_{}
	, color_{r, g, b}
{}


void Light::update(const ShaderProgram *program) const
{
	GLuint location{ program->getLocation("light.position") };
	glUniform3f(location, position_.x, position_.y, position_.z);

	location = program->getLocation("light.ambient");
	glUniform3f(location, color_.r, color_.g, color_.b);

	location = program->getLocation("light.diffuse");
	glUniform3f(location, color_.r, color_.g, color_.b);

	location = program->getLocation("light.specular");
	glUniform3f(location, color_.r, color_.g, color_.b);
}