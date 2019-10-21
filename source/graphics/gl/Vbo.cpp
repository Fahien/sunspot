#include "sunspot/graphics/gl/Vbo.h"


namespace sunspot::graphics::gl
{


namespace
{

} // namespace anonymous


Vbo::Vbo( const gltfspot::Gltf::Accessor& accessor, const gltfspot::Mesh::Primitive::Semantic semantic )
{
	glGenBuffers( 1, &handle );
	bind();
	glBufferData( GL_ARRAY_BUFFER, accessor.get_size(), accessor.get_data(), GL_STATIC_DRAW );
}


Vbo::Vbo( Vbo&& other )
: handle { other.handle }
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