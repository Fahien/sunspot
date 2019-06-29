#pragma once

#include "sunspot/core/GlfwWindow.h"
#include "sunspot/core/Gui.h"
#include "sunspot/core/Collisions.h"
#include "sunspot/graphics/Graphics.h"
#include "sunspot/core/Scene.h"

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
	GlfwWindow& get_window() { return window; }

	graphics::Graphics& get_graphics() { return graphics; }

	ImGui& get_gui() { return gui; }

	Scene& get_scene() { return scene; }

	void add( Entity& e );

	void handle( input::Input&& in );

	void loop();

  private:
	const float compute_delta_time();

	Time time = {};

	GlfwWindow window = { *this };
	ImGui      gui    = { window };

	Collisions collisions = {};
	graphics::Graphics graphics   = {};

	Scene scene;
};


}  // namespace sunspot
