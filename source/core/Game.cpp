#include "sunspot/core/Game.h"

#include "sunspot/entity/Entity.h"


namespace sunspot
{


void Game::handle( input::Input&& in )
{
	for ( auto& entity : entities )
	{
		entity->Handle( in );
	}
}


const float Game::compute_delta_time()
{
	time.current = static_cast<float>( glfwGetTime() );
	const auto delta = time.current - time.last;
	time.last = time.current;
	return delta;
}


void Game::loop()
{
	while ( !window.IsClosing() )
	{
		window.PollEvents();
		window.UpdateSize();
		
		collisions.Update();

		auto delta = compute_delta_time();

		gui.Update( delta );

		for ( auto& entity : entities )
		{
			entity->Update( delta );
		}

		graphics.Draw();

		gui.Draw();

		window.SwapBuffers();
	}
}


} // namespace sunspot
