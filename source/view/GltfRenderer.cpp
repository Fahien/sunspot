#include "view/GltfRenderer.h"
#include "Graphics.h"
#include "ShaderProgram.h"

using namespace std;
using namespace sunspot;
using namespace gltfspot;


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


void GltfRenderer::draw(const ShaderProgram& shader, const Gltf::Node* const pNode)
{
	// Render its children
	for (auto pChild : pNode->children)
	{
		draw(shader, pChild);
	}

	// Render the node
	if (pNode->pMesh)
	{
		auto& mesh = mMeshes[pNode->pMesh];
		mesh.SetMatrix(pNode->matrix);
		mMeshes[pNode->pMesh].Draw(shader);
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
