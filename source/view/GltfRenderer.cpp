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
		for ( auto& m : gltf.get_meshes() )
		{
			GltfMesh mesh{ gltf, m };
			m_Meshes.emplace( &m, move( mesh ) );
		}

		cameras.clear();
		for ( auto& c : gltf.get_cameras() )
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


void GltfRenderer::draw( const graphics::shader::Program& shader, const gst::Light& light, const mst::Mat4& transform )
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


void GltfRenderer::draw( const graphics::shader::Program& shader, const Node& node, const Mat4& transform )
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
	if ( node.mesh )
	{
		auto& mesh = m_Meshes[node.mesh];
		for ( auto& primitive : mesh.GetPrimitives() )
		{
			primitive.SetMatrix( tTransform );
			primitive.Draw( shader );
		}
	}

	// Whether it is a camera
	if ( node.camera )
	{
		draw( shader, *node.camera, tTransform );
	}

	// Whether it has a light
	if ( node.light )
	{
		draw( shader, *node.light, tTransform );
	}
}


mst::Mat4 create_projection_matrix( const gst::Camera& camera )
{
	mst::Mat4 projection;

	if ( camera.type == gst::Camera::Type::Perspective )
	{
		auto y = camera.perspective.yfov;
		auto a = camera.perspective.aspect_ratio;
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

void GltfRenderer::draw( const graphics::shader::Program& shader, const gst::Camera& camera,
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
