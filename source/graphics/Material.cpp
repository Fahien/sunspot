#include <iostream>
#include <sstream>

#include <logspot/Log.h>

#include "sunspot/graphics/Material.h"
#include "sunspot/graphics/Shader.h"
#include "sunspot/graphics/Texture.h"

using namespace std;
namespace lst = logspot;


namespace sunspot::graphics
{
Material::Material( const string& n )
    : name{ n }
{
}

void Material::bind( const shader::Program& shader ) const
{
	// Bind PBR base colour
	glUniform3f( shader.get_location( "material.color" ), color.r, color.g, color.b );

	// Bind PBR base color texture
	glUniform1i( shader.get_location( "material.hasColorTexture" ), color_texture != nullptr );
	if ( color_texture )
	{
		glUniform1i( shader.get_location( "material.colorTexture" ), 0 );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, color_texture->getId() );
	}

	// Bind PBR metallic factor
	glUniform1f( shader.get_location( "material.metallic" ), metallic );

	// Bind PBR roughness factor
	glUniform1f( shader.get_location( "material.roughness" ), roughness );

	// Bind PBR ambient occlusion
	glUniform1f( shader.get_location( "material.ambientOcclusion" ), ambient_occlusion );
}

}  // namespace sunspot::graphics
