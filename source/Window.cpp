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
const std::string Window::tag{ "Window" };


Window::Window( Game& g, const std::string& t, const mst::Size& window_size, const bool stereoscopic )
    : game{ g }
    , title{ t }
    , window{ /* .size = */ window_size }
    , m_MonitorSize{ window_size }
    , m_FrameSize{ window_size }
    , mStereoscopic{ stereoscopic }
    , mFullscreen{ false }
    , mCurrentTime{ 0.0f }
    , mLastTime{ 0.0f }
    , mDeltaTime{ 0.0f }
    , mCursor{}
    , m_pCamera{ nullptr }
    , mBaseProgram{ nullptr }
    , mLight{ nullptr }
    , mObjs{}
    , mQuadProgram{ nullptr }
    , mDepthProgram{ nullptr }
    , mQuad{ nullptr }
{
}


Window::~Window() {}


void Window::initGlew()
{
	glewExperimental = GL_TRUE;  // Initialize GLEW and handle error
	if ( glewInit() != GLEW_OK )
	{
		throw GlewException( tag );
	}
}


void Window::handleInput( input::Input&& i )
{
	game.handle( std::move( i ) );
}


}  // namespace sunspot
