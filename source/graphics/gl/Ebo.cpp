#include "sunspot/graphics/gl/Ebo.h"

#include <gltfspot/Gltf.h>


namespace sunspot::graphics::gl
{

namespace
{

GLenum to_gl_component_type( const gltfspot::Gltf::Accessor::ComponentType& type )
{
	switch ( type )
	{
		case gltfspot::Gltf::Accessor::ComponentType::BYTE:
			return GL_BYTE;
		case gltfspot::Gltf::Accessor::ComponentType::FLOAT:
			return GL_FLOAT;
		case gltfspot::Gltf::Accessor::ComponentType::SHORT:
			return GL_SHORT;
		case gltfspot::Gltf::Accessor::ComponentType::UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
		case gltfspot::Gltf::Accessor::ComponentType::UNSIGNED_SHORT:
			return GL_UNSIGNED_SHORT;
		case gltfspot::Gltf::Accessor::ComponentType::UNSIGNED_INT:
			return GL_UNSIGNED_INT;
		default:
			assert( "Invalid Component Type" );
			return GL_BYTE;
	}
}

} // namespace anonymous


Ebo::Ebo( const gltfspot::Mesh::Primitive& primitive )
{
	auto indices_index = primitive.indices;
	if( indices_index < 0 )
	{
		// The p has no indices
		// Should be rendered with drawArrays()
		return;
	}

	// Generate an element buffer for indices
	glGenBuffers( 1, &handle );
	assert( handle > 0 && "Element buffer object is not valid" );

	// Get the accessor
	auto& indices_accessor = primitive.mesh->model->GetAccessors()[indices_index];

	// Number of indices
	indices_count = static_cast<GLsizei>( indices_accessor.count );

	// Type of indices
	indices_type = to_gl_component_type( indices_accessor.componentType );

	// Indices offset
	indices_offset = reinterpret_cast<const void*>( indices_accessor.byteOffset );

	// Upload data into the buffer
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, handle );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices_accessor.get_size(), indices_accessor.get_data(), GL_STATIC_DRAW );
}


Ebo::Ebo( const gltfspot::Shape& shape )
{
	if ( auto rect = dynamic_cast<const gltfspot::Box*>( &shape ) )
	{
		// Gen indices
		glGenBuffers( 1, &handle );
		std::vector<GLuint> indices = {
			// front
			0, 1, 1, 2, 2, 3, 3, 0,
			// back
			4, 5, 5, 6, 6, 7, 7, 4,
			// sides
			0, 4, 3, 7, 1, 5, 2, 6
		};
		indices_count = indices.size();
		indices_type  = GL_UNSIGNED_INT;

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, handle );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( indices[0] ), indices.data(), GL_STATIC_DRAW );
	}
}


Ebo::Ebo( Ebo&& other )
: handle { other.handle }
, indices_count { other.indices_count }
, indices_type { other.indices_type }
, indices_offset {other.indices_offset }
{
	other.handle = 0;
}


Ebo::~Ebo()
{
	if ( handle > 0 )
	{
		glDeleteBuffers( 1, &handle );
	}
}


} // namespace sunspot::graphics::gl
