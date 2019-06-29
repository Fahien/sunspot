#include "sunspot/core/GlfwWindow.h"

#include <iostream>

#include <logspot/Log.h>

#include "Camera.h"
#include "sunspot/core/Game.h"

namespace lst = logspot;

namespace sunspot
{
GlfwWindow::GlfwWindow( Game& game, const char* title, const mst::Size windowSize, const bool stereoscopic )
    : Window::Window{ game, title, windowSize, stereoscopic },
      context{ { 3, 3 }, GLFW_OPENGL_CORE_PROFILE },
      monitor{ nullptr },
      videomode{ nullptr },
      handle{ nullptr }
{
	// Initialize GLFW and handle error
	if ( glfwInit() != GLFW_TRUE )
	{
		throw GlfwException{ tag, "Could not initialize GLFW" };
	}
	// Set the error callback
	glfwSetErrorCallback( []( int error, const char* description ) {
		lst::Log::error( "%s: %s [%d]", tag.c_str(), description, error );
	} );

	// Get the primary monitor
	monitor = glfwGetPrimaryMonitor();
	if ( monitor == nullptr )  // Handle error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not get primary monitor" };
	}

	// Get the video mode for the monitor
	videomode = glfwGetVideoMode( monitor );
	if ( videomode == nullptr )  // Handle error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not get video mode" };
	}
	m_MonitorSize.width  = videomode->width;
	m_MonitorSize.height = videomode->height;

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, context.version.major );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, context.version.minor );

	// Hard constraints
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
	glfwWindowHint( GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, context.profile );

	glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DECORATED, GLFW_TRUE );
	glfwWindowHint( GLFW_FOCUSED, GLFW_TRUE );
	glfwWindowHint( GLFW_RED_BITS, videomode->redBits );
	glfwWindowHint( GLFW_GREEN_BITS, videomode->greenBits );
	glfwWindowHint( GLFW_BLUE_BITS, videomode->blueBits );
	glfwWindowHint( GLFW_REFRESH_RATE, videomode->refreshRate );

	// Create a window object
	handle = glfwCreateWindow( window.size.width, window.size.height, m_Title, nullptr, nullptr );
	if ( handle == nullptr )  // Handle window creation error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not create GLFW window" };
	}
	glfwSetWindowUserPointer( handle, this );
	glfwMakeContextCurrent( handle );

	// Hide the cursor and capture it, perfect for an FPS camera system
	// glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Listen to mouse events
	glfwSetMouseButtonCallback( handle, []( GLFWwindow* handle, int button, int action, int mods ) {
		auto win = reinterpret_cast<GlfwWindow*>( glfwGetWindowUserPointer( handle ) );
		win->handleMouse( action );
	} );
	glfwSetCursorPosCallback( handle, []( GLFWwindow* handle, double xpos, double ypos ) {
		auto win = reinterpret_cast<GlfwWindow*>( glfwGetWindowUserPointer( handle ) );
		win->handleMouse( xpos, ypos );
	} );

	// Listen to keyboard events
	glfwSetKeyCallback( handle, []( GLFWwindow* handle, int key, int /* scancode */, int action, int /* mode */ ) {
		auto win = reinterpret_cast<GlfwWindow*>( glfwGetWindowUserPointer( handle ) );
		win->handleInput( key, action );
	} );

	try  // Initialize glew
	{
		Window::initGlew();
	} catch ( const GlewException& )  // Handle error
	{
		glfwDestroyWindow( handle );
		glfwTerminate();
		throw;
	}

	UpdateSize();
	glfwSwapInterval( 1 );  // Vsync

	lst::Log::info( "%s created\n\tOpenGL %s\n\tGLFW %s\n\tFrame size %dx%d\n", tag.c_str(),
	                       glGetString( GL_VERSION ), glfwGetVersionString(), m_FrameSize.width, m_FrameSize.height );
}


GlfwWindow::~GlfwWindow()
{
	if ( handle != nullptr )
	{
		glfwDestroyWindow( handle );
	}
	glfwTerminate();
	lst::Log::info( "%s: destroyed", tag.c_str() );  // TODO remove debug log
}


void GlfwWindow::SetClosing( const bool closing ) const
{
	if ( closing )
	{
		glfwSetWindowShouldClose( handle, GLFW_TRUE );
	}
}

void GlfwWindow::handleMouse( const int action )
{
	if ( action == GLFW_PRESS )
	{
		mAction = input::Action::PRESS;
	} else if ( action == GLFW_RELEASE )
	{
		mAction = input::Action::RELEASE;
	}
	Window::handleInput( input::Input{ mType, mKey, mAction, mPosition } );
}


void GlfwWindow::handleMouse( const double x, const double y )  // TODO comment
{
	mCursor.setPosition( static_cast<float>( x ), static_cast<float>( y ) );
	mType       = input::Type::MOTION;
	mPosition.x = static_cast<float>( x );
	mPosition.y = static_cast<float>( y );
	Window::handleInput( input::Input{ mType, mKey, mAction, mPosition } );
}


const input::Input GlfwWindow::pollInput()
{
	glfwPollEvents();
	return input::Input{};
}


void GlfwWindow::handleInput( const int key, const int action )  // TODO comment
{
	if ( action == GLFW_PRESS )
	{
		mAction = input::Action::PRESS;
	} else if ( action == GLFW_RELEASE )
	{
		mAction = input::Action::RELEASE;
	} else if ( action == GLFW_REPEAT )
	{
		mAction = input::Action::REPEAT;
	}

	switch ( key )
	{
		case GLFW_KEY_A:
			mKey = input::Key::A;
			break;
		case GLFW_KEY_Q:
			mKey = input::Key::Q;
			break;
		case GLFW_KEY_W:
			mKey = input::Key::W;
			break;
		case GLFW_KEY_E:
			mKey = input::Key::E;
			break;
		case GLFW_KEY_S:
			mKey = input::Key::S;
			break;
		case GLFW_KEY_D:
			mKey = input::Key::D;
			break;
		case GLFW_KEY_UP:
			mKey = input::Key::UP;
			break;
		case GLFW_KEY_LEFT:
			mKey = input::Key::LEFT;
			break;
		case GLFW_KEY_RIGHT:
			mKey = input::Key::RIGHT;
			break;
		case GLFW_KEY_DOWN:
			mKey = input::Key::DOWN;
			break;
		case GLFW_KEY_F:
			mKey = input::Key::NONE;
			if ( action == GLFW_PRESS )
			{
				toggleFullscreen();
			}
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose( handle, GLFW_TRUE );
			break;
		default:
			mKey = input::Key::NONE;
			break;
	}

	mType = input::Type::KEY;
	Window::handleInput( input::Input{ mType, mKey, mAction, mPosition } );
}


void GlfwWindow::toggleFullscreen()
{
	monitor   = glfwGetPrimaryMonitor();
	videomode = glfwGetVideoMode( monitor );
	if ( mFullscreen )  // Use start-up values for windowed mode
	{
		glfwSetWindowMonitor( handle, nullptr,
		                      videomode->width / 2 - window.size.width / 2,    // X center
		                      videomode->height / 2 - window.size.height / 2,  // Y center
		                      window.size.width, window.size.height, videomode->refreshRate );
	} else  // Set window size for fullscreen-windowed mode to the desktop resolution
	{
		glfwSetWindowMonitor( handle, monitor,
		                      0,  // left
		                      0,  // top
		                      videomode->width, videomode->height, videomode->refreshRate );
	}
	glfwSwapInterval( 1 );  // Vsync
	mFullscreen = !mFullscreen;
}


void GlfwWindow::loop()  // TODO comment
{
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);
#ifdef SST_PROFILING
	std::cout << "#Frame\tPass1\tPass2.1\tPass2.2\tTotal\tOverhead\n";
#endif
	while ( !glfwWindowShouldClose( handle ) )
	{
		glfwPollEvents();
		Window::render();
		glfwSwapBuffers( handle );
	}
}


void GlfwWindow::UpdateSize()
{
	glfwGetFramebufferSize( handle, &m_FrameSize.width, &m_FrameSize.height );
	GetGame().get_graphics().set_viewport( { {}, m_FrameSize } );
}


}  // namespace sunspot
