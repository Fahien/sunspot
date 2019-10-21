#include "sunspot/graphics/gl/Vao.h"

#include <gltfspot/Gltf.h>

#include "sunspot/graphics/gl/Renderer.h"


namespace sunspot::graphics::gl
{

namespace
{

GLenum to_gl_mode( const gltfspot::Mesh::Primitive::Mode& mode )
{
	switch ( mode )
	{
		case gltfspot::Mesh::Primitive::Mode::POINTS:
			return GL_POINTS;
		case gltfspot::Mesh::Primitive::Mode::LINES:
			return GL_LINES;
		case gltfspot::Mesh::Primitive::Mode::LINE_LOOP:
			return GL_LINE_LOOP;
		case gltfspot::Mesh::Primitive::Mode::LINE_STRIP:
			return GL_LINE_STRIP;
		case gltfspot::Mesh::Primitive::Mode::TRIANGLES:
			return GL_TRIANGLES;
		case gltfspot::Mesh::Primitive::Mode::TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
		case gltfspot::Mesh::Primitive::Mode::TRIANGLE_FAN:
			return GL_TRIANGLE_FAN;
		default:
			return GL_TRIANGLES;
	}
}


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


GLuint create_handle()
{
	// Generate a vertex array
	GLuint vao = 0;
	glGenVertexArrays( 1, &vao );
	assert( vao > 0 && "Vertex array object is not valid" );
	glBindVertexArray( vao );
	return vao;
}


} // namespace anonymous


Vao::Vao( Renderer& renderer, const gltfspot::Mesh::Primitive& primitive )
: renderer { &renderer }
, handle { create_handle() }
, ebo { primitive }
, mode { to_gl_mode( primitive.mode ) }
{
	auto& model = *primitive.mesh->model;

	// Vertex attributes
	for ( auto [semantic, accessor_index] : primitive.attributes )
	{
		auto& accessor = model.GetAccessors().at( accessor_index );
		auto& vbo      = renderer.get_vbo( accessor, semantic );
		vbo.bind();

		auto type   = to_gl_component_type( accessor.componentType );
		auto offset = reinterpret_cast<GLvoid*>( accessor.byteOffset );
		auto stride = static_cast<GLsizei>( accessor.get_stride() );

		if ( semantic == gltfspot::Mesh::Primitive::Semantic::NORMAL )
		{
			glEnableVertexAttribArray( 1 );  // Vertex Normals
			glVertexAttribPointer( 1, 3, type, GL_FALSE, stride, offset );
		}
		else if ( semantic == gltfspot::Mesh::Primitive::Semantic::POSITION )
		{
			glEnableVertexAttribArray( 0 );  // Vertex Position
			glVertexAttribPointer( 0, 3, type, GL_FALSE, stride, offset );
			vertex_count += 3;
		}
		else if ( semantic == gltfspot::Mesh::Primitive::Semantic::TEXCOORD_0 )
		{
			glEnableVertexAttribArray( 2 );  // Tex coords
			glVertexAttribPointer( 2, 2, type, GL_FALSE, stride, offset );
		}
		else if ( semantic == gltfspot::Mesh::Primitive::Semantic::COLOR_0 )
		{
			has_vertex_colors = true;
			glEnableVertexAttribArray( 3 );  // Color
			glVertexAttribPointer( 3, 4, type, GL_FALSE, stride, offset );
		}
	}

	glBindVertexArray( 0 );  // Unbind vao
}


Vao::Vao( Vao&& other )
: renderer { other.renderer }
, handle { other.handle }
, ebo { std::move( other.ebo ) }
, mode { other.mode }
, vertex_count { other.vertex_count }
, has_vertex_colors { other.has_vertex_colors }
{
	other.handle = 0;
}


Vao::~Vao()
{
	if ( handle > 0 )
	{
		glDeleteVertexArrays( 1, &handle );
	}
}


void Vao::draw()
{
	glBindVertexArray( handle );
	if ( ebo.indices_count > 0 )
	{
		glDrawElements( mode, ebo.indices_count, ebo.indices_type, ebo.indices_offset );
	}
	else
	{
		glDrawArrays( mode, 0, vertex_count );
	}
	glBindVertexArray( 0 );
}


} // namespace sunspot::graphics::gl
