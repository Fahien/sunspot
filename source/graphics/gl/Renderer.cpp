#include "sunspot/graphics/gl/Renderer.h"

#include <tuple>
#include <mathspot/Math.h>

#include "view/GltfPrimitive.h"
#include "sunspot/graphics/Shader.h"
#include "sunspot/util/Util.h"


namespace sunspot::graphics::gl
{


Renderer::Renderer( gst::Gltf* gltf )
{
	if ( gltf )
	{
		set_gltf( *gltf );
	}
}

void Renderer::set_gltf( gst::Gltf& g )
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


void Renderer::draw( const shader::Program& shader, const gst::Light& light, const mst::Mat4& transform )
{
	GLuint location{ shader.get_location( "pointLightActive" ) };
	glUniform1i( location, true );

	location = shader.get_location( "pLight.position" );
	glUniform3fv( location, 1, &transform.matrix[12] );

	location = shader.get_location( "pLight.ambient" );
	glUniform3fv( location, 1, &light.color.x );

	location = shader.get_location( "pLight.diffuse" );
	glUniform3fv( location, 1, &light.color.x );

	location = shader.get_location( "pLight.specular" );
	glUniform3fv( location, 1, &light.color.x );
}



void Renderer::bind( gst::Texture& component )
{
	const graphics::Texture* texture = nullptr;

	// Check if texture is in the cache
	auto it = textures.find( &component );
	if ( it != std::end( textures ) )
	{
		texture = &it->second;
	}
	else // Create texture
	{
		if ( component.source->uri != "" )
		{
			auto uri = gltf->GetPath() + "/" + component.source->uri;
			auto tex = graphics::Texture { uri, graphics::TextureType::DIFFUSE };
			auto res = textures.emplace( &component, std::move( tex ) );
			texture = &( res.first->second );
		}
		else
		{
			auto& buffer_view = gltf->GetBufferViews().at( component.source->buffer_view );
			auto  tex         = graphics::Texture { graphics::SoilData{ buffer_view, *gltf }, graphics::TextureType::DIFFUSE };
			auto  res         = textures.emplace( &component, std::move( tex ) );
			texture = &( res.first->second );
		}
	}

	glBindTexture( GL_TEXTURE_2D, texture->get_id() );
}


void Renderer::draw( const shader::Program& shader, const gst::Material& material )
{
	auto& pbr = material.pbr_metallic_roughness;

	// Bind PBR base colour
	glUniform3fv( shader.get_location( "material.color" ), 1, pbr.base_color_factor.data() );

	// Bind PBR base color texture
	glUniform1i( shader.get_location( "material.hasColorTexture" ), pbr.base_color_texture != nullptr );
	if ( pbr.base_color_texture )
	{
		glUniform1i( shader.get_location( "material.colorTexture" ), 0 );
		glActiveTexture( GL_TEXTURE0 );
		bind( *pbr.base_color_texture );
	}

	// Bind PBR metallic factor
	glUniform1f( shader.get_location( "material.metallic" ), pbr.metallic_factor );

	// Bind PBR roughness factor
	glUniform1f( shader.get_location( "material.roughness" ), pbr.roughness_factor );

	// Bind PBR ambient occlusion
	glUniform1f( shader.get_location( "material.ambientOcclusion" ), 0.25f );
}


Vao& Renderer::get_vao( const gst::Mesh::Primitive& primitive )
{
	Vao* ret = nullptr;

	// Get it from the cache
	auto it = vaos.find( &primitive );
	if ( it != std::end( vaos ) )
	{
		ret = &it->second;
	}
	else // Create a new one
	{
		Vao vao { *this, primitive };
		auto res = vaos.emplace( &primitive, std::move( vao ) );
		ret = &res.first->second;
	}

	return *ret;
}


Vbo& Renderer::get_vbo( const gst::Gltf::Accessor& accessor, const gst::Mesh::Primitive::Semantic semantic )
{
	Vbo* ret = nullptr;

	// Get it from the cache
	auto it = vbos.find( accessor.bufferView );
	if ( it != std::end( vbos ) )
	{
		ret = &it->second;
	}
	else // Create a new one
	{
		Vbo vbo { accessor, semantic };
		auto res = vbos.emplace( accessor.bufferView, std::move( vbo ) );
		ret = &res.first->second;
	}

	return *ret;
}


void Renderer::draw( const shader::Program& shader, const gst::Mesh::Primitive& primitive, const mst::Mat4& transform )
{
	// Bind transform matrix
	glUniformMatrix4fv( shader.get_location( "model" ), 1, GL_FALSE, transform.matrix );

	if ( primitive.material )
	{
		draw( shader, *primitive.material );
	}

	auto& vao = get_vao( primitive );
	if ( vao.has_vertex_colors )
	{
		glUniform1i( shader.get_location( "vertex.hasColor" ), true );
	}
	vao.draw();
}


void Renderer::draw( const shader::Program& shader, const gst::Mesh& mesh, const mst::Mat4& transform )
{
	for ( auto& primitive : mesh.primitives )
	{
		draw( shader, primitive, transform );
	}
}


void Renderer::draw( const shader::Program& shader, const gst::Node& node, const mst::Mat4& transform )
{
	// Current transform
	mst::Mat4 tTransform{ node.matrix };
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
		// auto& mesh = meshes[node.mesh];
		// mesh.draw( shader, tTransform );
		draw( shader, *node.mesh, tTransform );
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
	if ( render_bounds && node.bounds )
	{
		glDisable( GL_DEPTH_TEST );
		shape_shader.use();
		auto& shape = shapes[node.bounds_index];
		shape.draw( shape_shader, tTransform );
		shader.use();
		glEnable( GL_DEPTH_TEST );
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


void Renderer::draw( const shader::Program& shader, const gst::Camera& camera, const mst::Mat4& transform )
{
	auto view = transform;
	view[12] *= -1;
	view[13] *= -1;
	view[14] *= -1;

	auto location = shader.get_location( "view" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &view[0] );

	shape_shader.use();
	location = shape_shader.get_location( "view" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &view[0] );
	shader.use();

	auto projection = create_projection_matrix( camera );

	location = shader.get_location( "projection" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &projection[0] );

	shape_shader.use();
	location = shape_shader.get_location( "projection" );
	glUniformMatrix4fv( location, 1, GL_FALSE, &projection[0] );
	shader.use();

	location = shader.get_location( "camera.position" );
	glUniform3fv( location, 1, &transform[12] );
}

void Renderer::draw( const shader::Program& shader )
{
	auto pScene = gltf->GetScene();

	for ( auto node : pScene->nodes )
	{
		draw( shader, *node );
	}
}


} // namespace sunspot::graphics::gl
