#include <Gltf.h>

#include "ShaderProgram.h"
#include "view/GltfPrimitive.h"
#include "view/GltfMesh.h"


using namespace std;
using namespace gltfspot;
using namespace sunspot;

GltfMesh::GltfMesh(GltfMesh&& other)
:	mName      { move(other.mName)       }
,	mPrimitives{ move(other.mPrimitives) }
{}


GltfMesh::GltfMesh(Gltf& model, Gltf::Mesh& mesh)
: mName{ mesh.name }
{
	for (auto p : mesh.primitives)
	{
		GltfPrimitive primitive{ model, p };
		mPrimitives.push_back(move(primitive));
	}
}


void GltfMesh::Draw(const ShaderProgram& program) const
{
	for (auto& primitive : mPrimitives)
	{
		primitive.Draw(program);
	}
}
