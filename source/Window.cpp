#include "Window.h"

#include <iostream>

#ifdef SST_PROFILING
#include <chrono>
#endif

#include "Camera.h"
#include "Quad.h"
#include "SunSpotConfig.h"
#include "component/Model.h"
#include "entity/Entity.h"
#include "sunspot/core/Game.h"
#include "sunspot/graphics/Gl.h"
#include "sunspot/graphics/Light.h"
#include "sunspot/graphics/Shader.h"
#include "view/GltfCamera.h"
#include "view/GltfRenderer.h"

using namespace mathspot;

namespace sunspot
{
const std::string tag{ "Window" };


Window::Window( Game& g, const std::string& t, const mst::Size& window_size )
    : game{ g }
    , title{ t }
    , window{ /* .size = */ window_size }
    , monitor_size{ window_size }
    , frame_size{ window_size }
    , fullscreen{ false }
{
}

void Window::init_glew()
{
	glewExperimental = GL_TRUE;  // Initialize GLEW and handle error
	if ( glewInit() != GLEW_OK )
	{
		throw GlewException( tag );
	}
}


void Window::handle( input::Input&& i )
{
	game.handle( std::move( i ) );
}


}  // namespace sunspot
