#include <tuple>

#include "view/GltfRenderer.h"
#include "view/GltfPrimitive.h"

#include "Graphics.h"
#include "ShaderProgram.h"
#include <mathspot/Math.h>

using namespace std;
using namespace mathspot;
using namespace gltfspot;
using namespace sunspot;


GltfRenderer::GltfRenderer(Gltf&& gltf)
:	m_Gltf { move(gltf) }
{
	for (auto& m : m_Gltf.GetMeshes())
	{
		GltfMesh mesh { m_Gltf, m };
		m_Meshes.emplace(&m, move(mesh));
	}
}


GltfRenderer::GltfRenderer(GltfRenderer&& other)
: m_Gltf   { move(other.m_Gltf)   }
, m_Meshes { move(other.m_Meshes) }
{}


GltfRenderer::~GltfRenderer()
{}


void GltfRenderer::Draw(const ShaderProgram& shader,
                        const Gltf::Node* pNode,
                        const Mat4& transform)
{
	// Current transform
	Mat4 tTransform { pNode->matrix };
	tTransform.Translate(pNode->translation);
	tTransform.Rotate(pNode->rotation);
	tTransform = transform * tTransform;

	// Render its children
	for (auto pChild : pNode->children)
	{
		Draw(shader, pChild, tTransform);
	}

	// Render the node
	if (pNode->pMesh)
	{
		auto& mesh = m_Meshes[pNode->pMesh];
		for (auto& primitive : mesh.GetPrimitives())
		{
			primitive.SetMatrix(tTransform);
			primitive.Draw(shader);
		}
	}
}


void GltfRenderer::Draw(const ShaderProgram& shader)
{
	auto pScene = m_Gltf.GetScene();

	for(auto pNode : pScene->nodes)
	{
		Draw(shader, pNode);
	}
}
