#include "sunspot/graphics/Graphics.h"


namespace sunspot::graphics
{
void Graphics::draw( gltfspot::Gltf& gltf )
{
	renderer.set_gltf( gltf );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport( viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  // Clear color and depth buffer
	program->use();

	// Draw every node in the scene
	renderer.draw( *program );
}

}  // namespace sunspot::graphics
