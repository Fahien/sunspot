#include "view/GltfRenderer.h"
#include "Graphics.h"
#include "ShaderProgram.h"

using namespace std;
using namespace sunspot;
using namespace gltfspot;


GltfRenderer::GltfRenderer(Gltf& model)
: mMaterial{}
{
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);
	glGenBuffers(1, &mEbo);

	glBindVertexArray(mVao);

	// Indices
	{
		unsigned indicesIndex = model.GetMeshes().at(0).primitives.at(0).indices;
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

	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	// Vertex attributes
	{
		bool bound{ false };
		auto& attributes = model.GetMeshes().at(0).primitives.at(0).attributes;
		for (auto& attribute : attributes)
		{
			unsigned accessorIndex = attribute.second;
			auto& accessor = model.GetAccessors().at(accessorIndex);
			unsigned bufferViewIndex = accessor.bufferView;
			Gltf::BufferView& bufferView = model.GetBufferViews().at(bufferViewIndex);
			vector<char>& buffer = model.GetBuffer(bufferView.buffer);

			assert(bufferView.target == Gltf::BufferView::Target::ARRAY_BUFFER);
			if (!bound)
			{
				// TODO check i've already bound this buffer
				glBindBuffer(GL_ARRAY_BUFFER, mVbo);
				glBufferData(GL_ARRAY_BUFFER,
							 bufferView.byteLength,
							 &(buffer[bufferView.byteOffset]),
							 GL_STATIC_DRAW);
				bound = true;
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
		}
	}

	glBindVertexArray(0); // Unbind vao

	auto material = model.GetMaterials().at(0);
	const auto& color = material.pbrMetallicRoughness.baseColorFactor;

	mMaterial.color.r = color[0];
	mMaterial.color.g = color[1];
	mMaterial.color.b = color[2];

	mMaterial.metallic  = material.pbrMetallicRoughness.metallicFactor;
	mMaterial.roughness = material.pbrMetallicRoughness.roughnessFactor;
	mMaterial.ambientOcclusion = 0.25f;
}


GltfRenderer::~GltfRenderer()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mEbo);
	glDeleteBuffers(1, &mVbo);
}


void GltfRenderer::Draw(const ShaderProgram& shader) const
{
	// Bind transform matrix
	glUniformMatrix4fv(shader.getLocation("model"), 1, GL_FALSE, mathspot::Mat4::identity.matrix);

	mMaterial.bind(shader);

	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}
