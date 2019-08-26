#include <gltfspot/Gltf.h>

#include "ShaderProgram.h"
#include "view/GltfMesh.h"

namespace sunspot
{


GltfMesh::GltfMesh( gltfspot::Gltf& model, gltfspot::Mesh& mesh )
    : name{ mesh.name }
{
	for ( auto& p : mesh.primitives )
	{
		auto primitive = GltfPrimitive{ model, p };
		primitives.push_back( std::move( primitive ) );
	}
}


void GltfMesh::draw( const graphics::shader::Program& program, const mathspot::Mat4& transform ) const
{
	for ( auto& primitive : primitives )
	{
		primitive.draw( program, transform );
	}
}

}
