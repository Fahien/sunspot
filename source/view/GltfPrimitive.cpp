#include "view/GltfPrimitive.h"
#include "Graphics.h"
#include "ShaderProgram.h"

using namespace std;
using namespace sunspot;
using namespace mathspot;
using namespace gltfspot;


GLenum to_gl_mode(const Gltf::Mesh::Primitive::Mode& mode)
{
	switch (mode)
	{
		case Gltf::Mesh::Primitive::Mode::POINTS        : return GL_POINTS        ;
		case Gltf::Mesh::Primitive::Mode::LINES         : return GL_LINES         ;
		case Gltf::Mesh::Primitive::Mode::LINE_LOOP     : return GL_LINE_LOOP     ;
		case Gltf::Mesh::Primitive::Mode::LINE_STRIP    : return GL_LINE_STRIP    ;
		case Gltf::Mesh::Primitive::Mode::TRIANGLES     : return GL_TRIANGLES     ;
		case Gltf::Mesh::Primitive::Mode::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		case Gltf::Mesh::Primitive::Mode::TRIANGLE_FAN  : return GL_TRIANGLE_FAN  ;
		default: return GL_TRIANGLES;
	}
}

GLenum to_gl_component_type(const Gltf::Accessor::ComponentType& type)
{
	switch (type)
	{
		case Gltf::Accessor::ComponentType::BYTE          : return GL_BYTE          ;
		case Gltf::Accessor::ComponentType::FLOAT         : return GL_FLOAT         ;
		case Gltf::Accessor::ComponentType::SHORT         : return GL_SHORT         ;
		case Gltf::Accessor::ComponentType::UNSIGNED_BYTE : return GL_UNSIGNED_BYTE ;
		case Gltf::Accessor::ComponentType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
		case Gltf::Accessor::ComponentType::UNSIGNED_INT  : return GL_UNSIGNED_INT  ;
		default: assert("Invalid Component Type"); return GL_BYTE;
	}
}


GltfPrimitive::GltfPrimitive(GltfPrimitive&& other)
:	mHasVao         { other.mHasVao          }
,	mVao            { other.mVao             }
,	mVbos           { move(other.mVbos)      }
,	mHasEbo         { other.mHasEbo          }
,	mEbo            { other.mEbo             }
,	mMode           { other.mMode            }
,	mIndicesCount   { other.mIndicesCount    }
,	mIndicesType    { other.mIndicesType     }
,	mIndicesOffset  { other.mIndicesOffset   }
,	mMatrix         { other.mMatrix          }
,	mHasVertexColors{ other.mHasVertexColors }
,	mMaterial       { move(other.mMaterial)  }
,	mTextures       { move(other.mTextures)  }
{
	other.mHasVao = false;
	other.mHasEbo = false;
	assert(other.mVbos.empty());
	assert(other.mTextures.empty());
}


void GltfPrimitive::readIndices(Gltf& model, Gltf::Mesh::Primitive& primitive)
{
	// Generate an element buffer for indices
	glGenBuffers(1, &mEbo);
	mHasEbo = true;

	// Get the accessor
	unsigned indicesIndex = primitive.indices;
	auto& indicesAccessor = model.GetAccessors()[indicesIndex];

	// Number of indices
	mIndicesCount = indicesAccessor.count;

	// Type of indices
	mIndicesType = to_gl_component_type(indicesAccessor.componentType);
	
	// Indices offset
	mIndicesOffset = reinterpret_cast<const void*>(indicesAccessor.byteOffset);

	// Get the bufferview
	unsigned bufferViewIndex     = indicesAccessor.bufferView;
	Gltf::BufferView& bufferView = model.GetBufferViews()[bufferViewIndex];
	assert(bufferView.target == Gltf::BufferView::Target::ELEMENT_ARRAY_BUFFER);

	// Get the buffer
	unsigned bufferIndex = bufferView.buffer;
	vector<char>& buffer = model.GetBuffer(bufferIndex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 bufferView.byteLength,
				 &(buffer[bufferView.byteOffset]),
				 GL_STATIC_DRAW);
}


GltfPrimitive::GltfPrimitive(Gltf& model, Gltf::Mesh::Primitive& primitive)
:	mMode  { to_gl_mode(primitive.mode) }
,	mMatrix{ Mat4::identity.matrix      }
{
	// Generate a vertex array
	glGenVertexArrays(1, &mVao);
	mHasVao = true;
	glBindVertexArray(mVao);

	// Indices
	readIndices(model, primitive);

	// Vertex attributes
	for (auto& attribute : primitive.attributes)
	{
		unsigned accessorIndex = attribute.second;
		auto& accessor = model.GetAccessors().at(accessorIndex);
		unsigned bufferViewIndex = accessor.bufferView;
		Gltf::BufferView& bufferView = model.GetBufferViews().at(bufferViewIndex);
		vector<char>& buffer = model.GetBuffer(bufferView.buffer);

		assert(bufferView.target == Gltf::BufferView::Target::ARRAY_BUFFER);

		auto it = mVbos.find(bufferViewIndex);
		if (it == mVbos.end())
		{
			auto pair = mVbos.emplace(bufferViewIndex, 0);
			auto& vbo = pair.first->second;
			glGenBuffers(1, &vbo);
			// TODO check i've already bound this buffer
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER,
						 bufferView.byteLength,
						 &(buffer[bufferView.byteOffset]),
						 GL_STATIC_DRAW);
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, it->second);
		}

		GLenum componentType{ to_gl_component_type(accessor.componentType) };

		if (attribute.first == Gltf::Mesh::Primitive::Semantic::NORMAL)
		{
			glEnableVertexAttribArray(1); // Vertex Normals
			GLvoid* offset{ reinterpret_cast<GLvoid*>(accessor.byteOffset) };
			glVertexAttribPointer(1, 3, componentType, GL_FALSE, bufferView.byteStride, offset);
		}
		else if (attribute.first == Gltf::Mesh::Primitive::Semantic::POSITION)
		{
			glEnableVertexAttribArray(0); // Vertex Position
			GLvoid* offset{ reinterpret_cast<GLvoid*>(accessor.byteOffset) };
			glVertexAttribPointer(0, 3, componentType, GL_FALSE, bufferView.byteStride, offset);
		}
		else if (attribute.first == Gltf::Mesh::Primitive::Semantic::TEXCOORD_0)
		{
			glEnableVertexAttribArray(2); // Tex coords
			GLvoid* offset{ reinterpret_cast<GLvoid*>(accessor.byteOffset) };
			glVertexAttribPointer(2, 2, componentType, GL_FALSE, bufferView.byteStride, offset);
		}
		else if (attribute.first == Gltf::Mesh::Primitive::Semantic::COLOR_0)
		{
			mHasVertexColors = true;
			glEnableVertexAttribArray(3); // Color
			GLvoid* offset{ reinterpret_cast<GLvoid*>(accessor.byteOffset) };
			glVertexAttribPointer(3, 4, componentType, GL_FALSE, bufferView.byteStride, offset);
		}
	}

	glBindVertexArray(0); // Unbind vao

	// TODO extract method
	auto materialIndex = primitive.material;
	auto& materials = model.GetMaterials();
	if (materials.empty())
	{
		return;
	}
	auto material = model.GetMaterials().at(materialIndex);
	const auto& color = material.pbrMetallicRoughness.baseColorFactor;

	mMaterial.color.r = color[0];
	mMaterial.color.g = color[1];
	mMaterial.color.b = color[2];

	mMaterial.metallic  = material.pbrMetallicRoughness.metallicFactor;
	mMaterial.roughness = material.pbrMetallicRoughness.roughnessFactor;
	mMaterial.ambientOcclusion = 0.25f;

	if (material.pbrMetallicRoughness.baseColorTexture)
	{
		const auto& t = material.pbrMetallicRoughness.baseColorTexture;
		const auto& it = mTextures.find(t);
		if (it == mTextures.end())
		{
			const auto& uri = model.GetPath() + "/" + t->source->uri;
			// TODO use these
			t->sampler->magFilter;
			t->sampler->minFilter;
			t->sampler->wrapS;
			t->sampler->wrapT;
			Texture texture{ uri, TextureType::DIFFUSE };
			auto r = mTextures.emplace(t, move(texture));
			mMaterial.colorTexture = &(r.first->second);
		}
		else
		{
			mMaterial.colorTexture = &(it->second);
		}
	}
}


GltfPrimitive::~GltfPrimitive()
{
	if (mHasVao)
	{
		glDeleteVertexArrays(1, &mVao);
	}

	if (mHasEbo)
	{
		glDeleteBuffers(1, &mEbo);
	}

	for (auto& pair : mVbos)
	{
		glDeleteBuffers(1, &(pair.second));
	}
}


void GltfPrimitive::SetMatrix(const Mat4& matrix)
{
	mMatrix = matrix;
}


void GltfPrimitive::Draw(const ShaderProgram& shader) const
{
	// Bind transform matrix
	glUniformMatrix4fv(shader.getLocation("model"), 1, GL_FALSE, mMatrix.matrix);
	// Bind PBR base color texture
	glUniform1i(shader.getLocation("vertex.hasColor"), mHasVertexColors);

	mMaterial.bind(shader);

	glBindVertexArray(mVao);
	glDrawElements(mMode, mIndicesCount, mIndicesType, mIndicesOffset);
	glBindVertexArray(0);
}
