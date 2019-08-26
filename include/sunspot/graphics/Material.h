#pragma once

#include <fstream>

#include "Color.h"
#include "sunspot/graphics/Shader.h"


namespace sunspot::graphics
{
class Texture;

struct Material
{
	Material( const std::string& n = "Unknown" );

	/// Bind the material to the shader
	void bind( const shader::Program& shader ) const;

	friend std::ostream& operator<<( std::ostream& os, const Material& mtl );

	std::string name;

	Color color             = { 1.0f, 1.0f, 0.0f };
	float metallic          = 1.0f;
	float roughness         = 1.0f;
	float ambient_occlusion = 0.25f;

	Texture* color_texture = nullptr;
};


}  // namespace sunspot::graphics
