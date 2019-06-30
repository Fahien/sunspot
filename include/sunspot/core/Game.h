#ifndef SST_GAME_H_
#define SST_GAME_H_

#include <nlohmann/json.hpp>

#include "sunspot/core/Collisions.h"
#include "sunspot/core/GlfwWindow.h"
#include "sunspot/core/Gui.h"
#include "sunspot/core/Scene.h"
#include "sunspot/graphics/Graphics.h"
#include "sunspot/util/Cube.h"

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

	gltfspot::Gltf::Scene& get_scene() { return *gltf.GetScene(); }

	void set_gltf( gltfspot::Gltf&& g ) { gltf = std::move( g ); }

	void add( Entity& e );

	void handle( input::Input&& in );

	void loop();

  private:
	const float compute_delta_time();

	Time time = {};

	GlfwWindow window = { *this };
	ImGui      gui    = { window };

	gltfspot::Gltf gltf = gltfspot::Gltf( nlohmann::json::parse( cube ) );

	Collisions         collisions = {};
	graphics::Graphics graphics   = {};
};


}  // namespace sunspot

#endif  // SST_GAME_H_
