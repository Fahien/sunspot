#include "view/GltfPrimitive.h"
#include "sunspot/graphics/Shader.h"

namespace sunspot
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


GltfPrimitive::GltfPrimitive( GltfPrimitive&& other )
    : mHasVao{ other.mHasVao }
    , mVao{ other.mVao }
    , mVbos{ std::move( other.mVbos ) }
    , mHasEbo{ other.mHasEbo }
    , mEbo{ other.mEbo }
    , mMode{ other.mMode }
    , mIndicesCount{ other.mIndicesCount }
    , mIndicesType{ other.mIndicesType }
    , mIndicesOffset{ other.mIndicesOffset }
    , vertex_count{ other.vertex_count }
    , mHasVertexColors{ other.mHasVertexColors }
    , primitive{ other.primitive }
    , mMaterial{ std::move( other.mMaterial ) }
    , mTextures{ std::move( other.mTextures ) }
{
	other.mHasVao   = false;
	other.mHasEbo   = false;
	other.primitive = nullptr;
	assert( other.mVbos.empty() );
	assert( other.mTextures.empty() );
}

GltfPrimitive& GltfPrimitive::operator=( GltfPrimitive&& other )
{
	mHasVao          = other.mHasVao;
	mVao             = other.mVao;
	mVbos            = std::move( other.mVbos );
	mHasEbo          = other.mHasEbo;
	mEbo             = other.mEbo;
	mMode            = other.mMode;
	mIndicesCount    = other.mIndicesCount;
	mIndicesType     = other.mIndicesType;
	mIndicesOffset   = other.mIndicesOffset;
	vertex_count     = other.vertex_count;
	mHasVertexColors = other.mHasVertexColors;
	primitive        = other.primitive;
	mMaterial        = std::move( other.mMaterial );
	mTextures        = std::move( other.mTextures );

	other.mHasVao   = false;
	other.mHasEbo   = false;
	other.primitive = nullptr;

	assert( other.mVbos.empty() );
	assert( other.mTextures.empty() );

	return *this;
}

void GltfPrimitive::read_indices( gltfspot::Gltf& model, gltfspot::Mesh::Primitive& p )
{
	auto indicesIndex = p.indices;
	if ( indicesIndex < 0 )
	{
		// The p has no indices
		// Should be rendered with drawArrays()
		return;
	}

	// Generate an element buffer for indices
	glGenBuffers( 1, &mEbo );
	mHasEbo = true;

	// Get the accessor
	auto& indicesAccessor = model.GetAccessors()[indicesIndex];

	// Number of indices
	mIndicesCount = static_cast<GLsizei>( indicesAccessor.count );

	// Type of indices
	mIndicesType = to_gl_component_type( indicesAccessor.componentType );

	// Indices offset
	mIndicesOffset = reinterpret_cast<const void*>( indicesAccessor.byteOffset );

	// Get the bufferview
	auto  bufferViewIndex = indicesAccessor.bufferView;
	auto& bufferView      = model.GetBufferViews()[bufferViewIndex];

	// Get the buffer
	auto               bufferIndex = bufferView.buffer;
	std::vector<char>& buffer      = model.GetBuffer( bufferIndex );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mEbo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, bufferView.byteLength, &( buffer[bufferView.byteOffset] ), GL_STATIC_DRAW );
}

GltfPrimitive::GltfPrimitive( gltfspot::Gltf& model, gltfspot::Shape& shape )
    : mMode{ to_gl_mode( gltfspot::Mesh::Primitive::Mode::LINES ) }
{
	// Generate a vertex array
	glGenVertexArrays( 1, &mVao );
	mHasVao = true;
	glBindVertexArray( mVao );

	if ( auto rect = dynamic_cast<gltfspot::Box*>( &shape ) )
	{
		// Gen indices
		glGenBuffers( 1, &mEbo );
		mHasEbo                     = true;
		std::vector<GLuint> indices = { // front
			                            0, 1, 1, 2, 2, 3, 3, 0,
			                            // back
			                            4, 5, 5, 6, 6, 7, 7, 4,
			                            // sides
			                            0, 4, 3, 7, 1, 5, 2, 6
		};
		mIndicesCount = indices.size();
		mIndicesType  = GL_UNSIGNED_INT;

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mEbo );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( indices[0] ), indices.data(), GL_STATIC_DRAW );

		auto [it, success] = mVbos.emplace( 0, 0 );
		glGenBuffers( 1, &it->second );

		auto& a = rect->a;
		auto& b = rect->b;

		std::vector<mathspot::Vec3> points = {
			{ a.x, a.y, b.z }, { b.x, a.y, b.z }, b, { a.x, b.y, b.z }, a, { b.x, a.y, a.z },
			{ b.x, b.y, a.z }, { a.x, b.y, a.z }
		};

		vertex_count = points.size();

		glBindBuffer( GL_ARRAY_BUFFER, it->second );
		glBufferData( GL_ARRAY_BUFFER, points.size() * sizeof( mathspot::Vec3 ), points.data(), GL_STATIC_DRAW );

		glEnableVertexAttribArray( 0 );  // Vertex Position
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}

	glBindVertexArray( 0 );  // Unbind vao
}

GltfPrimitive::GltfPrimitive( gltfspot::Gltf& model, gltfspot::Mesh::Primitive& p )
    : mMode{ to_gl_mode( p.mode ) }
    , primitive{ &p }
{
	// Generate a vertex array
	glGenVertexArrays( 1, &mVao );
	mHasVao = true;
	glBindVertexArray( mVao );

	// Indices
	read_indices( model, p );

	// Vertex attributes
	for ( auto& attribute : p.attributes )
	{
		unsigned                    accessorIndex   = attribute.second;
		auto&                       accessor        = model.GetAccessors().at( accessorIndex );
		auto                        bufferViewIndex = accessor.bufferView;
		gltfspot::Gltf::BufferView& bufferView      = model.GetBufferViews().at( bufferViewIndex );
		std::vector<char>&          buffer          = model.GetBuffer( bufferView.buffer );

		auto it = mVbos.find( bufferViewIndex );
		if ( it == mVbos.end() )
		{
			auto  pair = mVbos.emplace( bufferViewIndex, 0 );
			auto& vbo  = pair.first->second;
			glGenBuffers( 1, &vbo );
			// TODO check i've already bound this buffer
			glBindBuffer( GL_ARRAY_BUFFER, vbo );
			glBufferData( GL_ARRAY_BUFFER, bufferView.byteLength, &( buffer[bufferView.byteOffset] ), GL_STATIC_DRAW );
		}
		else
		{
			glBindBuffer( GL_ARRAY_BUFFER, it->second );
		}

		GLenum componentType{ to_gl_component_type( accessor.componentType ) };

		if ( attribute.first == gltfspot::Mesh::Primitive::Semantic::NORMAL )
		{
			glEnableVertexAttribArray( 1 );  // Vertex Normals
			GLvoid* offset{ reinterpret_cast<GLvoid*>( accessor.byteOffset ) };
			auto    stride = static_cast<GLsizei>( bufferView.byteStride );
			glVertexAttribPointer( 1, 3, componentType, GL_FALSE, stride, offset );
		}
		else if ( attribute.first == gltfspot::Mesh::Primitive::Semantic::POSITION )
		{
			glEnableVertexAttribArray( 0 );  // Vertex Position
			GLvoid* offset{ reinterpret_cast<GLvoid*>( accessor.byteOffset ) };
			auto    stride = static_cast<GLsizei>( bufferView.byteStride );
			glVertexAttribPointer( 0, 3, componentType, GL_FALSE, stride, offset );

			vertex_count += 3;
		}
		else if ( attribute.first == gltfspot::Mesh::Primitive::Semantic::TEXCOORD_0 )
		{
			glEnableVertexAttribArray( 2 );  // Tex coords
			GLvoid* offset{ reinterpret_cast<GLvoid*>( accessor.byteOffset ) };
			auto    stride = static_cast<GLsizei>( bufferView.byteStride );
			glVertexAttribPointer( 2, 2, componentType, GL_FALSE, stride, offset );
		}
		else if ( attribute.first == gltfspot::Mesh::Primitive::Semantic::COLOR_0 )
		{
			mHasVertexColors = true;
			glEnableVertexAttribArray( 3 );  // Color
			GLvoid* offset{ reinterpret_cast<GLvoid*>( accessor.byteOffset ) };
			auto    stride = static_cast<GLsizei>( bufferView.byteStride );
			glVertexAttribPointer( 3, 4, componentType, GL_FALSE, stride, offset );
		}
	}

	glBindVertexArray( 0 );  // Unbind vao

	// TODO extract method
	if ( auto material = p.material )
	{
		const auto& color = material->pbr_metallic_roughness.base_color_factor;

		mMaterial.color.r = color[0];
		mMaterial.color.g = color[1];
		mMaterial.color.b = color[2];

		mMaterial.metallic          = material->pbr_metallic_roughness.metallic_factor;
		mMaterial.roughness         = material->pbr_metallic_roughness.roughness_factor;
		mMaterial.ambient_occlusion = 0.25f;

		if ( material->pbr_metallic_roughness.base_color_texture )
		{
			const auto& t  = material->pbr_metallic_roughness.base_color_texture;
			const auto& it = mTextures.find( t );
			if ( it == mTextures.end() )
			{
				if ( t->source->uri != "" )
				{
					const auto&       uri = model.GetPath() + "/" + t->source->uri;
					graphics::Texture texture{ uri, graphics::TextureType::DIFFUSE };
					auto              r     = mTextures.emplace( t, std::move( texture ) );
					mMaterial.color_texture = &( r.first->second );
				}
				else
				{
					auto&             buffer_view = model.GetBufferViews().at( t->source->buffer_view );
					graphics::Texture texture{ graphics::SoilData{ buffer_view, model }, graphics::TextureType::DIFFUSE };
					auto              r     = mTextures.emplace( t, std::move( texture ) );
					mMaterial.color_texture = &( r.first->second );
				}
			}
			else
			{
				mMaterial.color_texture = &( it->second );
			}
		}
	}
}


GltfPrimitive::~GltfPrimitive()
{
	if ( mHasVao )
	{
		glDeleteVertexArrays( 1, &mVao );
	}

	if ( mHasEbo )
	{
		glDeleteBuffers( 1, &mEbo );
	}

	for ( auto& pair : mVbos )
	{
		glDeleteBuffers( 1, &( pair.second ) );
	}
}


void GltfPrimitive::bind_material( const graphics::shader::Program& shader, gltfspot::Material& material ) const
{
	auto& pbr = material.pbr_metallic_roughness;

	// Bind PBR base colour
	glUniform3fv( shader.get_location( "material.color" ), 1, pbr.base_color_factor.data() );

	// Bind PBR base color texture
	glUniform1i( shader.get_location( "material.hasColorTexture" ), pbr.base_color_texture != nullptr );
	if ( pbr.base_color_texture )
	{
		glUniform1i( shader.get_location( "material.colorTexture" ), 0 );
		glActiveTexture( GL_TEXTURE0 );
		auto& texture = mTextures.at( pbr.base_color_texture );
		glBindTexture( GL_TEXTURE_2D, texture.getId() );
	}

	// Bind PBR metallic factor
	glUniform1f( shader.get_location( "material.metallic" ), pbr.metallic_factor );

	// Bind PBR roughness factor
	glUniform1f( shader.get_location( "material.roughness" ), pbr.roughness_factor );

	// Bind PBR ambient occlusion
	glUniform1f( shader.get_location( "material.ambientOcclusion" ), 0.25f );
}


void GltfPrimitive::draw( const graphics::shader::Program& shader, const mathspot::Mat4& transform ) const
{
	// Bind transform matrix
	glUniformMatrix4fv( shader.get_location( "model" ), 1, GL_FALSE, transform.matrix );
	// Bind PBR base color texture
	glUniform1i( shader.get_location( "vertex.hasColor" ), mHasVertexColors );

	if ( primitive && primitive->material )
	{
		bind_material( shader, *primitive->material );
	}

	glBindVertexArray( mVao );
	if ( mIndicesCount > 0 )
	{
		glDrawElements( mMode, mIndicesCount, mIndicesType, mIndicesOffset );
	}
	else
	{
		glDrawArrays( mMode, 0, vertex_count );
	}
	glBindVertexArray( 0 );
}

}  // namespace sunspot
