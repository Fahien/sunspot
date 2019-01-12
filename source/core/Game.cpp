#include "sunspot/core/Game.h"

#include "sunspot/entity/Entity.h"


namespace sunspot
{


void Game::Handle( input::Input&& in )
{
	for ( auto& pEntity : entities )
	{
		pEntity->Handle( in );
	}
}


const float Game::computeDeltaTime()
{
	time.current = static_cast<float>( glfwGetTime() );
	const auto delta = time.current - time.last;
	time.last = time.current;
	return delta;
}


void Game::Loop()
{
	while ( !window.IsClosing() )
	{
		window.PollEvents();
		window.UpdateSize();
		
		collisions.Update();

		auto delta = computeDeltaTime();

		gui.Update( delta );

		for ( auto pEntity : entities )
		{
			pEntity->Update( delta );
		}

		graphics.Draw();

		gui.Draw();

		window.SwapBuffers();
	}
}


} // namespace sunspot
