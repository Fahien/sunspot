#include "sunspot/graphics/Graphics.h"

#include "sunspot/component/Camera.h"

namespace sunspot::graphics
{
void Graphics::draw( gltfspot::Gltf& gltf )
{
	renderer.set_gltf( gltf );

	glEnable( GL_DEPTH_TEST );
	glViewport( viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  // Clear color and depth buffer
	program->Use();
	light->Update( *program );

	if ( camera )
	{
		if ( auto perspective_camera = camera->get<component::PerspectiveCamera>() )
		{
			perspective_camera->Update( *program );
		}
	}

	// Draw every node in the scene
	for ( auto node : gltf.GetScene()->nodes )
	{
		renderer.draw( *program, node );
	}
}

}  // namespace sunspot::graphics
