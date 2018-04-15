#include "view/GltfMesh.h"
#include "Graphics.h"
#include "ShaderProgram.h"

using namespace std;
using namespace sunspot;
using namespace mathspot;
using namespace gltfspot;


GltfMesh::GltfMesh(GltfMesh&& other)
:	mMatrix         { other.mMatrix }
,	mHasVertexColors{ other.mHasVertexColors }
,	mMaterial       { move(other.mMaterial) }
,	mHasVao         { other.mHasVao }
,	mVao            { other.mVao }
,	mHasEbo         { other.mHasEbo }
,	mEbo            { other.mEbo }
,	mVbos           { move(other.mVbos) }
,	mIndicesCount   { other.mIndicesCount }
,	mTextures       { move(other.mTextures) }
{
	other.mHasVao = false;
	other.mHasEbo = false;
	assert(other.mVbos.empty());
	assert(other.mTextures.empty());
}


GltfMesh::GltfMesh(Gltf& model, Gltf::Mesh& mesh)
:	mMatrix         { Mat4::identity.matrix }
,	mHasVertexColors{ false }
,	mMaterial       {}
,	mHasVao         { false }
,	mVao            {}
,	mHasEbo         { false }
,	mEbo            {}
,	mVbos           {}
,	mTextures       {}
{
	glGenVertexArrays(1, &mVao);
	mHasVao = true;

	glGenBuffers(1, &mEbo);
	mHasEbo = true;

	glBindVertexArray(mVao);

	// Indices
	{
		unsigned indicesIndex = mesh.primitives.at(0).indices;
		auto& accessor = model.GetAccessors().at(indicesIndex);
		mIndicesCount = accessor.count;
		unsigned bufferViewIndex = accessor.bufferView;
		Gltf::BufferView& bufferView = model.GetBufferViews().at(bufferViewIndex);
		vector<char>& buffer = model.GetBuffer(bufferView.buffer);

		assert(bufferView.target == Gltf::BufferView::Target::ELEMENT_ARRAY_BUFFER);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 bufferView.byteLength,
					 &(buffer[bufferView.byteOffset]),
					 GL_STATIC_DRAW);
	}

	// Vertex attributes
	{
		auto& attributes = model.GetMeshes().at(0).primitives.at(0).attributes;
		for (auto& attribute : attributes)
		{
			unsigned accessorIndex = attribute.second;
			auto& accessor = model.GetAccessors().at(accessorIndex);
			unsigned bufferViewIndex = accessor.bufferView;
			Gltf::BufferView& bufferView = model.GetBufferViews().at(bufferViewIndex);
			vector<char>& buffer = model.GetBuffer(bufferView.buffer);

			assert(bufferView.target == Gltf::BufferView::Target::ARRAY_BUFFER);

			auto& it = mVbos.find(bufferViewIndex);
			if (it == mVbos.end())
			{
				auto& pair = mVbos.emplace(bufferViewIndex, 0);
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

			if (attribute.first == Gltf::Mesh::Primitive::Semantic::NORMAL)
			{
				glEnableVertexAttribArray(1); // Vertex Normals
				GLvoid* offset{ reinterpret_cast<GLvoid*>(accessor.byteOffset) };
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, bufferView.byteStride, offset);
			}
			else if (attribute.first == Gltf::Mesh::Primitive::Semantic::POSITION)
			{
				glEnableVertexAttribArray(0); // Vertex Position
				GLvoid* offset{ reinterpret_cast<GLvoid*>(accessor.byteOffset) };
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferView.byteStride, offset);
			}
			else if (attribute.first == Gltf::Mesh::Primitive::Semantic::TEXCOORD_0)
			{
				glEnableVertexAttribArray(2); // Tex coords
				GLvoid* offset{ reinterpret_cast<GLvoid*>(accessor.byteOffset) };
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, bufferView.byteStride, offset);
			}
			else if (attribute.first == Gltf::Mesh::Primitive::Semantic::COLOR_0)
			{
				mHasVertexColors = true;
				glEnableVertexAttribArray(3); // Color
				GLvoid* offset{ reinterpret_cast<GLvoid*>(accessor.byteOffset) };
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, bufferView.byteStride, offset);
			}
		}
	}

	glBindVertexArray(0); // Unbind vao

	// TODO extract method
	auto material = model.GetMaterials().at(0);
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
			auto& r = mTextures.emplace(t, move(texture));
			mMaterial.colorTexture = &(r.first->second);
		}
		else
		{
			mMaterial.colorTexture = &(it->second);
		}
	}
}


GltfMesh::~GltfMesh()
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


void GltfMesh::SetMatrix(const Mat4& matrix)
{
	mMatrix = matrix;
}


void GltfMesh::Draw(const ShaderProgram& shader) const
{
	// Bind transform matrix
	glUniformMatrix4fv(shader.getLocation("model"), 1, GL_FALSE, mMatrix.matrix);
	// Bind PBR base color texture
	glUniform1i(shader.getLocation("vertex.hasColor"), mHasVertexColors);

	mMaterial.bind(shader);

	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}
