#include <gltfspot/Gltf.h>

#include "ShaderProgram.h"
#include "view/GltfMesh.h"
#include "view/GltfPrimitive.h"

using namespace std;
using namespace gltfspot;
using namespace sunspot;


GltfMesh::GltfMesh( GltfMesh&& other )
    : mName{ move( other.mName ) }
    , mPrimitives{ move( other.mPrimitives ) }
{
}


GltfMesh::GltfMesh( Gltf& model, Mesh& mesh )
    : mName{ mesh.name }
{
	for ( auto p : mesh.primitives )
	{
		GltfPrimitive primitive{ model, p };
		mPrimitives.push_back( move( primitive ) );
	}
}


void GltfMesh::Draw( const graphics::shader::Program& program ) const
{
	for ( auto& primitive : mPrimitives )
	{
		primitive.Draw( program );
	}
}
