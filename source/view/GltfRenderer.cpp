#include <tuple>

#include "view/GltfPrimitive.h"
#include "view/GltfRenderer.h"

#include <mathspot/Math.h>
#include "ShaderProgram.h"
#include "sunspot/graphics/Shader.h"

using namespace std;
using namespace mathspot;
using namespace gltfspot;
using namespace sunspot;


GltfRenderer::GltfRenderer( Gltf* gltf )
{
	if ( gltf )
	{
		set_gltf( *gltf );
	}
}

void GltfRenderer::set_gltf( Gltf& gltf )
{
	if ( m_Gltf != &gltf )
	{
		m_Gltf = &gltf;
		m_Meshes.clear();
		for ( auto& m : gltf.GetMeshes() )
		{
			GltfMesh mesh{ gltf, m };
			m_Meshes.emplace( &m, move( mesh ) );
		}
	}
}


GltfRenderer::GltfRenderer( GltfRenderer&& other )
    : m_Gltf{ move( other.m_Gltf ) }
    , m_Meshes{ move( other.m_Meshes ) }
{
	other.m_Gltf = nullptr;
}


GltfRenderer::~GltfRenderer() {}


void GltfRenderer::draw( const graphics::shader::Program& shader, const Gltf::Node* pNode, const Mat4& transform )
{
	// Current transform
	Mat4 tTransform{ pNode->matrix };
	tTransform.Translate( pNode->translation );
	tTransform.Rotate( pNode->rotation );
	tTransform.Scale( pNode->scale );
	tTransform = transform * tTransform;

	// Render its children
	if ( pNode->children.size() > 0 )
	{
		for ( auto pChild : pNode->children )
		{
			draw( shader, pChild, tTransform );
		}
	}

	// Render the node
	if ( pNode->pMesh )
	{
		auto& mesh = m_Meshes[pNode->pMesh];
		for ( auto& primitive : mesh.GetPrimitives() )
		{
			primitive.SetMatrix( tTransform );
			primitive.Draw( shader );
		}
	}
}


void GltfRenderer::draw( const graphics::shader::Program& shader )
{
	auto pScene = m_Gltf->GetScene();

	for ( auto pNode : pScene->nodes )
	{
		draw( shader, pNode );
	}
}
