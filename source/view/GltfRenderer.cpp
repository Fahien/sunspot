
#include "view/GltfRenderer.h"
#include "view/GltfPrimitive.h"

#include "Graphics.h"
#include "ShaderProgram.h"
#include "MathSpot.h"

using namespace std;
using namespace mathspot;
using namespace gltfspot;
using namespace sunspot;


GltfRenderer::GltfRenderer(Gltf&& gltf)
:	mGltf{ move(gltf) }
{
	for (Gltf::Mesh& m : mGltf.GetMeshes())
	{
		GltfMesh mesh{ mGltf, m };
		mMeshes.emplace(&m, move(mesh));
	}
}


GltfRenderer::GltfRenderer(GltfRenderer&& other)
: mGltf           { move(other.mGltf)      }
, mMeshes         { move(other.mMeshes)    }
{}


GltfRenderer::~GltfRenderer()
{}


void GltfRenderer::Draw(const ShaderProgram& shader,
                        const Gltf::Node* pNode,
                        const Mat4& transform)
{
	// Current transform
	Mat4 tTransform{ pNode->matrix };
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
		auto& mesh = mMeshes[pNode->pMesh];
		for (auto& primitive : mesh.GetPrimitives())
		{
			primitive.SetMatrix(tTransform);
			primitive.Draw(shader);
		}
	}
}


void GltfRenderer::Draw(const ShaderProgram& shader)
{
	auto pScene = mGltf.GetScene();
	for(auto pNode : pScene->nodes)
	{
		Draw(shader, pNode);
	}
}
