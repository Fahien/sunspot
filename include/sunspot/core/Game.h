#pragma once

#include "sunspot/core/GlfwWindow.h"
#include "sunspot/core/Gui.h"
#include "sunspot/system/Collision.h"
#include "sunspot/system/Graphic.h"

namespace sunspot
{


struct Time
{
	float current;
	float last;
};


class Game
{
  public:

	GlfwWindow& GetWindow() { return m_Window; }
	graphic::System& GetGraphics() { return m_Graphics; }

	void AddEntity( Entity& entity ) { m_Entities.push_back( &entity ); m_Collisions.Add( entity ); }

	void Handle( input::Input&& in );

	void Loop();

  private:
	const float computeDeltaTime();

	Time m_Time = {};

	GlfwWindow m_Window = { *this };
	ImGui      m_Gui    = { m_Window };

	system::Collision m_Collisions = {};
	graphic::System   m_Graphics   = {};

	std::vector<Entity*> m_Entities = {};
};


} // namespace sunspot
