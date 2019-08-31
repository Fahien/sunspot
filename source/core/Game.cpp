#include "sunspot/core/Game.h"

#include "sunspot/entity/Entity.h"


namespace sunspot
{
Game::Game( Config& c )
    : config{ c }
{
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
	scripts.handle( in );
}


const float Game::compute_delta_time()
{
	time.current     = window.get_time();
	const auto delta = time.current - time.last;
	time.last        = time.current;
	return delta;
}


void Game::loop()
{
	while ( !window.is_closing() )
	{
		window.poll_events();
		window.update_size();

		collisions.update( get_scene() );

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

		window.swap_buffers();
	}
}


}  // namespace sunspot
