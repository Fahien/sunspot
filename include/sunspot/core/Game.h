#pragma once

#include "sunspot/core/GlfwWindow.h"
#include "sunspot/core/Gui.h"
#include "sunspot/system/Collision.h"
#include "sunspot/system/graphics/Graphics.h"

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
	GlfwWindow& get_window()
	{
		return window;
	}

	graphics::Graphics& get_graphics()
	{
		return graphics;
	}

	ImGui& get_gui()
	{
		return gui;
	}

	void add_entity( Entity& entity )
	{
		entities.push_back( &entity );
		collisions.Add( entity );
	}

	const std::vector<Entity*> get_entities() const
	{
		return entities;
	}

	void handle( input::Input&& in );

	void loop();

  private:
	const float compute_delta_time();

	Time time = {};

	GlfwWindow window = { *this };
	ImGui      gui    = { window };

	system::Collision collisions = {};
	graphics::Graphics   graphics   = {};

	std::vector<Entity*> entities = {};
};


}  // namespace sunspot
