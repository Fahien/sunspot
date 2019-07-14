#include "sunspot/core/Game.h"

#include "sunspot/entity/Entity.h"


namespace sunspot
{
Game::Game( Config& c )
    : config{ c }
{
}

void Game::add( Entity& e )
{
	collisions.add( e );
}


void Game::set_size( const mathspot::Size& size )
{
	graphics.set_viewport( { {}, size } );

	for ( auto& camera : gltf.get_cameras() )
	{
		if ( camera.type == gst::Camera::Type::Perspective )
		{
			camera.perspective.aspect_ratio = static_cast<float>( size.width ) / size.height;
		}
	}
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

		scripts.update( gltf, delta );

		animations.update( delta, gltf );

		graphics.draw( gltf );

		editor.draw( gltf );

		gui.draw();

		window.SwapBuffers();
	}
}


}  // namespace sunspot
