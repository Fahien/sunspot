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

		cameras.clear();
		for ( auto& c : gltf.GetCameras() )
		{
			auto camera = GltfCamera::create( c );
			cameras.emplace( &c, std::move( camera ) );
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


void GltfRenderer::draw( const graphics::shader::Program& shader, const gst::Gltf::Light& light, const mst::Mat4& transform )
{
	GLuint location{ shader.GetLocation( "pointLightActive" ) };
	glUniform1i( location, true );

	location = shader.GetLocation( "pLight.position" );
	glUniform3fv( location, 1, &transform.matrix[12] );

	location = shader.GetLocation( "pLight.ambient" );
	glUniform3fv( location, 1, &light.color.x );

	location = shader.GetLocation( "pLight.diffuse" );
	glUniform3fv( location, 1, &light.color.x );

	location = shader.GetLocation( "pLight.specular" );
	glUniform3fv( location, 1, &light.color.x );
}


void GltfRenderer::draw( const graphics::shader::Program& shader, const Gltf::Node& node, const Mat4& transform )
{
	// Current transform
	Mat4 tTransform{ node.matrix };
	tTransform.Scale( node.scale );
	tTransform.Rotate( node.rotation );
	tTransform.Translate( node.translation );
	tTransform = transform * tTransform;

	// Render its children
	if ( node.children.size() > 0 )
	{
		for ( auto child : node.children )
		{
			draw( shader, *child, tTransform );
		}
	}

	// Render the node
	if ( node.pMesh )
	{
		auto& mesh = m_Meshes[node.pMesh];
		for ( auto& primitive : mesh.GetPrimitives() )
		{
			primitive.SetMatrix( tTransform );
			primitive.Draw( shader );
		}
	}

	// Whether it is a camera
	if ( node.pCamera )
	{
		draw( shader, *node.pCamera, tTransform );
	}

	// Whether it has a light
	if ( node.light )
	{
		draw( shader, *node.light, tTransform );
	}
}


mst::Mat4 create_projection_matrix( const gst::Gltf::Camera& camera )
{
	mst::Mat4 projection;

	if ( camera.type == gst::Gltf::Camera::Type::Perspective )
	{
		auto y = camera.perspective.yfov;
		auto a = camera.perspective.aspectRatio;
		auto n = camera.perspective.znear;
		auto f = camera.perspective.zfar;

		float cotfov{ 1.0f / std::tan( 0.5f * y ) };
		projection[0]  = cotfov / a;
		projection[5]  = cotfov;
		projection[10] = ( n + f ) / ( n - f );
		projection[14] = 2.0f * n * f / ( n - f);
		projection[11] = -1.0f;
	}
	else
	{
		// TODO implement orthographic camera
		assert( false && "Orthographic camera not supported" );
	}


	return projection;
}

void GltfRenderer::draw( const graphics::shader::Program& shader, const gst::Gltf::Camera& camera,
                         const mst::Mat4& transform )
{
	auto view = transform;
	view[12] *= -1;
	view[13] *= -1;
	view[14] *= -1;

	auto location = shader.GetLocation( "view" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &view[0] );

	auto projection = create_projection_matrix( camera );

	location = shader.GetLocation( "projection" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &projection[0] );

	location = shader.GetLocation( "camera.position" );
	glUniform3fv( location, 1, &transform[12] );
}

void GltfRenderer::draw( const graphics::shader::Program& shader )
{
	auto pScene = m_Gltf->GetScene();

	for ( auto node : pScene->nodes )
	{
		draw( shader, *node );
	}
}
