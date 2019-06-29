#include "sunspot/system/graphics/Light.h"
#include "sunspot/system/graphics/Shader.h"

namespace sunspot::graphics
{


Light::Light( const float r, const float g, const float b )
:	m_Ambient  { r, g, b }
,	m_Diffuse  { r, g, b }
,	m_Specular { r, g, b }
{}


void DirectionalLight::Update( const shader::Program& program ) const
{
	GLuint location{ program.GetLocation( "directionalLightActive" ) };
	glUniform1i(location, true);

	location = program.GetLocation( "dLight.direction" );
	glUniform3fv(location, 1, &m_Direction.x);

	location = program.GetLocation("dLight.ambient");
	glUniform3fv(location, 1, &m_Ambient.r);

	location = program.GetLocation("dLight.diffuse");
	glUniform3fv(location, 1, &m_Diffuse.r);

	location = program.GetLocation("dLight.specular");
	glUniform3fv(location, 1, &m_Specular.r);
}


void PointLight::Update(const shader::Program& program) const
{
	GLuint location{ program.GetLocation("pointLightActive") };
	glUniform1i(location, true);

	location = program.GetLocation("pLight.position");
	glUniform3fv(location, 1, &m_Position.x);

	location = program.GetLocation("pLight.ambient");
	glUniform3fv(location, 1, &m_Ambient.r);

	location = program.GetLocation("pLight.diffuse");
	glUniform3fv(location, 1, &m_Diffuse.r);

	location = program.GetLocation("pLight.specular");
	glUniform3fv(location, 1, &m_Specular.r);
}


} // namespace sunspot::graphics