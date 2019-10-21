#pragma once

#include <gltfspot/Mesh.h>

#include "sunspot/graphics/gl/Gl.h"


namespace sunspot::graphics::gl
{


/// @brief Element Buffer Object
struct Ebo
{
  public:
	Ebo( const gltfspot::Mesh::Primitive& p );

	Ebo( Ebo&& o );

	~Ebo();

	GLuint handle = 0;

	GLsizei indices_count = 0;

	GLenum indices_type = 0;

	const void* indices_offset = nullptr;
};


} // namespace sunspot::graphics::gl
