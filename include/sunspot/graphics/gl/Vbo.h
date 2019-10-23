#pragma once

#include "sunspot/graphics/gl/Gl.h"

#include <gltfspot/Gltf.h>


namespace sunspot::graphics::gl
{


struct Vbo
{
	Vbo( const gltfspot::Gltf::Accessor& a, gltfspot::Mesh::Primitive::Semantic s );

	Vbo( const void* data, size_t s );

	Vbo( Vbo&& v );

	~Vbo();

	void bind() const;

	GLuint handle = 0;
};


} // namespace sunspot::graphics::gl
