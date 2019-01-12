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

	GlfwWindow& GetWindow() { return window; }
	graphic::System& GetGraphics() { return graphics; }
	ImGui& GetGui() { return gui; }

	void AddEntity( Entity& entity ) { entities.push_back( &entity ); collisions.Add( entity ); }

	void Handle( input::Input&& in );

	void Loop();

  private:
	const float computeDeltaTime();

	Time time = {};

	GlfwWindow window = { *this };
	ImGui      gui    = { window };

	system::Collision collisions = {};
	graphic::System   graphics   = {};

	std::vector<Entity*> entities = {};
};


} // namespace sunspot
