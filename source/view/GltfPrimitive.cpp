#include "view/GltfPrimitive.h"
#include "sunspot/graphics/Shader.h"

using namespace std;
using namespace sunspot;
using namespace mathspot;
using namespace gltfspot;


GLenum to_gl_mode( const Mesh::Primitive::Mode& mode )
{
	switch ( mode )
	{
		case Mesh::Primitive::Mode::POINTS:
			return GL_POINTS;
		case Mesh::Primitive::Mode::LINES:
			return GL_LINES;
		case Mesh::Primitive::Mode::LINE_LOOP:
			return GL_LINE_LOOP;
		case Mesh::Primitive::Mode::LINE_STRIP:
			return GL_LINE_STRIP;
		case Mesh::Primitive::Mode::TRIANGLES:
			return GL_TRIANGLES;
		case Mesh::Primitive::Mode::TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
		case Mesh::Primitive::Mode::TRIANGLE_FAN:
			return GL_TRIANGLE_FAN;
		default:
			return GL_TRIANGLES;
	}
}

GLenum to_gl_component_type( const Gltf::Accessor::ComponentType& type )
{
	switch ( type )
	{
		case Gltf::Accessor::ComponentType::BYTE:
			return GL_BYTE;
		case Gltf::Accessor::ComponentType::FLOAT:
			return GL_FLOAT;
		case Gltf::Accessor::ComponentType::SHORT:
			return GL_SHORT;
		case Gltf::Accessor::ComponentType::UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
		case Gltf::Accessor::ComponentType::UNSIGNED_SHORT:
			return GL_UNSIGNED_SHORT;
		case Gltf::Accessor::ComponentType::UNSIGNED_INT:
			return GL_UNSIGNED_INT;
		default:
			assert( "Invalid Component Type" );
			return GL_BYTE;
	}
}


GltfPrimitive::GltfPrimitive( GltfPrimitive&& other )
    : mHasVao{ other.mHasVao }
    , mVao{ other.mVao }
    , mVbos{ move( other.mVbos ) }
    , mHasEbo{ other.mHasEbo }
    , mEbo{ other.mEbo }
    , mMode{ other.mMode }
    , mIndicesCount{ other.mIndicesCount }
    , mIndicesType{ other.mIndicesType }
    , mIndicesOffset{ other.mIndicesOffset }
    , vertex_count{ other.vertex_count }
    , mHasVertexColors{ other.mHasVertexColors }
    , mMaterial{ move( other.mMaterial ) }
    , mTextures{ move( other.mTextures ) }
{
	other.mHasVao = false;
	other.mHasEbo = false;
	assert( other.mVbos.empty() );
	assert( other.mTextures.empty() );
}


void GltfPrimitive::read_indices( Gltf& model, Mesh::Primitive& primitive )
{
	auto indicesIndex = primitive.indices;
	if ( indicesIndex < 0 )
	{
		// The primitive has no indices
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
	auto          bufferIndex = bufferView.buffer;
	vector<char>& buffer      = model.GetBuffer( bufferIndex );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mEbo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, bufferView.byteLength, &( buffer[bufferView.byteOffset] ), GL_STATIC_DRAW );
}

GltfPrimitive::GltfPrimitive( Gltf& model, Shape& shape )
    : mMode{ to_gl_mode( Mesh::Primitive::Mode::LINES ) }
{
	// Generate a vertex array
	glGenVertexArrays( 1, &mVao );
	mHasVao = true;
	glBindVertexArray( mVao );

	if ( auto rect = dynamic_cast<Box*>( &shape ) )
	{
		// Gen indices
		glGenBuffers( 1, &mEbo );
		mHasEbo                    = true;
		std::vector<GLuint> indices = { // front
			                           0, 1, 1, 2, 2, 3, 3, 0,
			                           // back
			                           4, 5, 5, 6, 6, 7, 7, 4,
			                           // sides
			                           0, 4, 3, 7, 1, 5, 2, 6
		};
		mIndicesCount = indices.size();
		mIndicesType = GL_UNSIGNED_INT;

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mEbo );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( indices[0] ), indices.data(), GL_STATIC_DRAW );

		auto [it, success] = mVbos.emplace( 0, 0 );
		glGenBuffers( 1, &it->second );

		auto& a = rect->a;
		auto& b = rect->b;

		std::vector<mst::Vec3> points = { { a.x, a.y, b.z }, { b.x, a.y, b.z }, b, { a.x, b.y, b.z }, a, { b.x, a.y, a.z },
			                              { b.x, b.y, a.z }, { a.x, b.y, a.z } };

		vertex_count = points.size();

		glBindBuffer( GL_ARRAY_BUFFER, it->second );
		glBufferData( GL_ARRAY_BUFFER, points.size() * sizeof( mst::Vec3 ), points.data(), GL_STATIC_DRAW );

		glEnableVertexAttribArray( 0 );  // Vertex Position
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}

	glBindVertexArray( 0 );  // Unbind vao
}

GltfPrimitive::GltfPrimitive( Gltf& model, Mesh::Primitive& primitive )
    : mMode{ to_gl_mode( primitive.mode ) }
{
	// Generate a vertex array
	glGenVertexArrays( 1, &mVao );
	mHasVao = true;
	glBindVertexArray( mVao );

	// Indices
	read_indices( model, primitive );

	// Vertex attributes
	for ( auto& attribute : primitive.attributes )
	{
		unsigned          accessorIndex   = attribute.second;
		auto&             accessor        = model.GetAccessors().at( accessorIndex );
		auto              bufferViewIndex = accessor.bufferView;
		Gltf::BufferView& bufferView      = model.GetBufferViews().at( bufferViewIndex );
		vector<char>&     buffer          = model.GetBuffer( bufferView.buffer );

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

		if ( attribute.first == Mesh::Primitive::Semantic::NORMAL )
		{
			glEnableVertexAttribArray( 1 );  // Vertex Normals
			GLvoid* offset{ reinterpret_cast<GLvoid*>( accessor.byteOffset ) };
			auto    stride = static_cast<GLsizei>( bufferView.byteStride );
			glVertexAttribPointer( 1, 3, componentType, GL_FALSE, stride, offset );
		}
		else if ( attribute.first == Mesh::Primitive::Semantic::POSITION )
		{
			glEnableVertexAttribArray( 0 );  // Vertex Position
			GLvoid* offset{ reinterpret_cast<GLvoid*>( accessor.byteOffset ) };
			auto    stride = static_cast<GLsizei>( bufferView.byteStride );
			glVertexAttribPointer( 0, 3, componentType, GL_FALSE, stride, offset );

			vertex_count += 3;
		}
		else if ( attribute.first == Mesh::Primitive::Semantic::TEXCOORD_0 )
		{
			glEnableVertexAttribArray( 2 );  // Tex coords
			GLvoid* offset{ reinterpret_cast<GLvoid*>( accessor.byteOffset ) };
			auto    stride = static_cast<GLsizei>( bufferView.byteStride );
			glVertexAttribPointer( 2, 2, componentType, GL_FALSE, stride, offset );
		}
		else if ( attribute.first == Mesh::Primitive::Semantic::COLOR_0 )
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
	auto  materialIndex = primitive.material;
	auto& materials     = model.GetMaterials();
	if ( materials.empty() )
	{
		return;
	}
	auto        material = model.GetMaterials().at( materialIndex );
	const auto& color    = material.pbrMetallicRoughness.baseColorFactor;

	mMaterial.color.r = color[0];
	mMaterial.color.g = color[1];
	mMaterial.color.b = color[2];

	mMaterial.metallic         = material.pbrMetallicRoughness.metallicFactor;
	mMaterial.roughness        = material.pbrMetallicRoughness.roughnessFactor;
	mMaterial.ambientOcclusion = 0.25f;

	if ( material.pbrMetallicRoughness.baseColorTexture )
	{
		const auto& t  = material.pbrMetallicRoughness.baseColorTexture;
		const auto& it = mTextures.find( t );
		if ( it == mTextures.end() )
		{
			if ( t->source->uri != "" )
			{
				const auto&       uri = model.GetPath() + "/" + t->source->uri;
				graphics::Texture texture{ uri, graphics::TextureType::DIFFUSE };
				auto              r    = mTextures.emplace( t, move( texture ) );
				mMaterial.colorTexture = &( r.first->second );
			}
			else
			{
				auto&             buffer_view = model.GetBufferViews().at( t->source->buffer_view );
				graphics::Texture texture{ graphics::SoilData{ buffer_view, model }, graphics::TextureType::DIFFUSE };
				auto              r    = mTextures.emplace( t, move( texture ) );
				mMaterial.colorTexture = &( r.first->second );
			}
		}
		else
		{
			mMaterial.colorTexture = &( it->second );
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


void GltfPrimitive::draw( const graphics::shader::Program& shader, const mst::Mat4& transform ) const
{
	// Bind transform matrix
	glUniformMatrix4fv( shader.GetLocation( "model" ), 1, GL_FALSE, transform.matrix );
	// Bind PBR base color texture
	glUniform1i( shader.GetLocation( "vertex.hasColor" ), mHasVertexColors );

	mMaterial.bind( shader );

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
