#include "Window.h"

#include <iostream>

#ifdef SST_PROFILING
#include <chrono>
#endif

#include "Camera.h"
#include "SunSpotConfig.h"
#include "sunspot/core/Game.h"
#include "sunspot/graphics/gl/Gl.h"

namespace sunspot
{


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
		throw GlewException( "Window" );
	}
}


void Window::handle( input::Input&& i )
{
	game.handle( std::move( i ) );
}


}  // namespace sunspot
