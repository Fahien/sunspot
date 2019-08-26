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

void GltfRenderer::set_gltf( Gltf& g )
{
	if ( gltf != &g )
	{
		gltf = &g;
		meshes.clear();
		for ( auto& m : g.get_meshes() )
		{
			meshes.emplace( &m, GltfMesh{ g, m } );
		}

		shapes.clear();
		for ( auto& s : g.get_shapes() )
		{
			shapes.emplace_back( g, *s );
		}

		cameras.clear();
		for ( auto& c : g.get_cameras() )
		{
			auto camera = GltfCamera::create( c );
			cameras.emplace( &c, std::move( camera ) );
		}
	}
}


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
		auto& mesh = meshes[node.mesh];
		mesh.draw( shader, tTransform );
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

	// Render the shape
	if ( node.bounds )
	{
		glDisable(GL_DEPTH_TEST);
		shape_shader.Use();
		auto& shape = shapes[node.bounds_index];
		shape.draw( shape_shader, tTransform );
		shader.Use();
		glEnable(GL_DEPTH_TEST);
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
		projection[14] = 2.0f * n * f / ( n - f );
		projection[11] = -1.0f;
	}
	else
	{
		// TODO implement orthographic camera
		assert( false && "Orthographic camera not supported" );
	}


	return projection;
}

void GltfRenderer::draw( const graphics::shader::Program& shader, const gst::Camera& camera, const mst::Mat4& transform )
{
	auto view = transform;
	view[12] *= -1;
	view[13] *= -1;
	view[14] *= -1;

	auto location = shader.GetLocation( "view" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &view[0] );

	shape_shader.Use();
	location = shape_shader.GetLocation( "view" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &view[0] );
	shader.Use();

	auto projection = create_projection_matrix( camera );

	location = shader.GetLocation( "projection" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &projection[0] );

	shape_shader.Use();
	location = shape_shader.GetLocation( "projection" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &projection[0] );
	shader.Use();

	location = shader.GetLocation( "camera.position" );
	glUniform3fv( location, 1, &transform[12] );
}

void GltfRenderer::draw( const graphics::shader::Program& shader )
{
	auto pScene = gltf->GetScene();

	for ( auto node : pScene->nodes )
	{
		draw( shader, *node );
	}
}
