#ifndef SST_GAME_H_
#define SST_GAME_H_

#include <nlohmann/json.hpp>

#include "sunspot/core/Animations.h"
#include "sunspot/core/Collisions.h"
#include "sunspot/core/Scripts.h"
#include "sunspot/core/Config.h"
#include "sunspot/core/GlfwWindow.h"
#include "sunspot/core/Gui.h"
#include "sunspot/core/Scene.h"
#include "sunspot/editor/Editor.h"
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
	Game( Config& config );

	GlfwWindow& get_window() { return window; }

	graphics::Graphics& get_graphics() { return graphics; }

	ImGui& get_gui() { return gui; }

	gltfspot::Gltf& get_gltf() { return gltf; }

	gltfspot::Gltf::Scene& get_scene() { return *gltf.GetScene(); }

	void set_gltf( gltfspot::Gltf&& g ) { gltf = std::move( g ); }

	void set_size( const mathspot::Size& s );

	void handle( input::Input&& in );

	void loop();

  private:
	const float compute_delta_time();

	Config& config;

	Time time = {};

	GlfwWindow window = { *this, config.project.name, config.window.size };
	ImGui      gui    = { window };
	Editor     editor;

	gltfspot::Gltf gltf = gltfspot::Gltf( nlohmann::json::parse( util::cube ) );

	Collisions         collisions = {};
	Scripts            scripts    = {};
	Animations         animations = {};
	graphics::Graphics graphics   = {};
};


}  // namespace sunspot

#endif  // SST_GAME_H_
