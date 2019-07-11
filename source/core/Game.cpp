#include "sunspot/core/Game.h"

#include "sunspot/entity/Entity.h"


namespace sunspot
{
void Game::add( Entity& e )
{
	collisions.add( e );
}

void Game::handle( input::Input&& in )
{
	for ( auto node : get_scene().nodes )
	{
		// TODO get script component to handle input
		// entity->Handle( in );
	}
}


const float Game::compute_delta_time()
{
	time.current     = static_cast<float>( glfwGetTime() );
	const auto delta = time.current - time.last;
	time.last        = time.current;
	return delta;
}


void Game::loop()
{
	while ( !window.IsClosing() )
	{
		window.PollEvents();
		window.UpdateSize();

		collisions.update();

		auto delta = compute_delta_time();

		gui.update( delta );

		for ( auto node : get_scene().nodes )
		{
			// Update node's components
			// entity->Update( delta );
		}

		animations.update( delta, gltf );

		graphics.draw( gltf );

		editor.draw( gltf );

		gui.draw();

		window.SwapBuffers();
	}
}


}  // namespace sunspot
