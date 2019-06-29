#include "sunspot/system/graphics/Graphics.h"

#include "sunspot/component/Camera.h"

namespace sunspot::graphics
{
void Graphics::draw()
{
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
			perspective_camera->get().Update( *program );
		}
	}

	for ( auto model : models )
	{
		model->GetRenderer().Draw( *program, &model->GetNode() );
	}
}

}  // namespace sunspot::graphics
