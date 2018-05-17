#include "view/GltfRenderer.h"
#include "view/GltfPrimitive.h"

#include "Logger.h"
#include "Graphics.h"
#include "ShaderProgram.h"
#include "MathSpot.h"

using namespace std;
using namespace logspot;
using namespace mathspot;
using namespace gltfspot;
using namespace sunspot;


GltfRenderer::GltfRenderer(Gltf& model)
:	mModel{ model }
,	mHasVertexColors{ false }
,	mMaterial{}
,	mMeshes{}
,	mTextures{}
{
	for (Gltf::Mesh& m : model.GetMeshes())
	{
		GltfMesh mesh{ model, m };
		mMeshes.emplace(&m, move(mesh));
	}
}


GltfRenderer::~GltfRenderer()
{}


void GltfRenderer::draw(const ShaderProgram& shader,
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
		draw(shader, pChild, tTransform);
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
	auto pScene = mModel.GetScene();
	for(auto pNode : pScene->nodes)
	{
		draw(shader, pNode);
	}
}
