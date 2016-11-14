#include "Light.h"
#include "ShaderProgram.h"


Light::Light(const float r, const float g, const float b)
	: color_{r, g, b}
{}


void Light::update(const ShaderProgram *program) const
{
	GLuint ambientLoc{ program->getLocation("light.ambient") };
	glUniform3f(ambientLoc, color_.r, color_.g, color_.b);
}