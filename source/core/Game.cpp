#include "sunspot/core/Game.h"

#include "sunspot/entity/Entity.h"


namespace sunspot
{


const float Game::computeDeltaTime()
{
	m_Time.current = static_cast<float>( glfwGetTime() );
	const auto delta = m_Time.current - m_Time.last;
	m_Time.last = m_Time.current;
	return delta;
}

void Game::Loop()
{
	while ( !m_Window.ShouldClose() )
	{
		m_Window.PollEvents();
		m_Window.UpdateSize();
		
		m_Collisions.Update();

		auto delta = computeDeltaTime();

		for ( auto pEntity : m_Entities )
		{
			pEntity->Update( delta );
		}

		m_Graphics.Draw();
		
		m_Window.SwapBuffers();
	}
}


} // namespace sunspot
