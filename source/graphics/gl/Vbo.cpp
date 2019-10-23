#include "sunspot/graphics/gl/Vbo.h"


namespace sunspot::graphics::gl
{


namespace
{

} // namespace anonymous


Vbo::Vbo( const void* data, const size_t size )
{
	assert( size > 0 && "Vertex buffer object data size should be greater than zero" );
	assert( data && "Vertex buffer object data should not be null" );

	glGenBuffers( 1, &handle );
	bind();
	glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
}

Vbo::Vbo( const gltfspot::Gltf::Accessor& accessor, const gltfspot::Mesh::Primitive::Semantic semantic )
:	Vbo { accessor.get_data(), accessor.get_size() }
{
}


Vbo::Vbo( Vbo&& other )
:	handle { other.handle }
{
	other.handle = 0;
}


Vbo::~Vbo()
{
	if ( handle > 0 )
	{
		glDeleteBuffers( 1, &handle );
	}
}


void Vbo::bind() const
{
	glBindBuffer( GL_ARRAY_BUFFER, handle );
}


} // namespace sunspot::graphics::gl