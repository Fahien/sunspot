#include "sunspot/graphics/Light.h"
#include "sunspot/graphics/Shader.h"

namespace sunspot::graphics
{
Light::Light( const float r, const float g, const float b )
    : m_Ambient{ r, g, b }
    , m_Diffuse{ r, g, b }
    , m_Specular{ r, g, b }
{
}


void DirectionalLight::Update( const shader::Program& program ) const
{
	GLuint location{ program.get_location( "directionalLightActive" ) };
	glUniform1i( location, true );

	location = program.get_location( "dLight.direction" );
	glUniform3fv( location, 1, &m_Direction.x );

	location = program.get_location( "dLight.ambient" );
	glUniform3fv( location, 1, &m_Ambient.r );

	location = program.get_location( "dLight.diffuse" );
	glUniform3fv( location, 1, &m_Diffuse.r );

	location = program.get_location( "dLight.specular" );
	glUniform3fv( location, 1, &m_Specular.r );
}


void PointLight::Update( const shader::Program& program ) const
{
	GLuint location{ program.get_location( "pointLightActive" ) };
	glUniform1i( location, true );

	location = program.get_location( "pLight.position" );
	glUniform3fv( location, 1, &m_Position.x );

	location = program.get_location( "pLight.ambient" );
	glUniform3fv( location, 1, &m_Ambient.r );

	location = program.get_location( "pLight.diffuse" );
	glUniform3fv( location, 1, &m_Diffuse.r );

	location = program.get_location( "pLight.specular" );
	glUniform3fv( location, 1, &m_Specular.r );
}


}  // namespace sunspot::graphics