#include "sunspot/core/GlfwWindow.h"

#include <iostream>

#include <logspot/Log.h>

#include "Camera.h"
#include "sunspot/core/Game.h"

namespace lst = logspot;

namespace sunspot
{
const std::string tag{ "GlfwWindow" };

GlfwWindow::GlfwWindow( Game& game, const std::string& title, const mst::Size& window_size )
    : Window::Window{ game, title, window_size }
    , context{ { 3, 3 }, GLFW_OPENGL_CORE_PROFILE }
{
	// Initialize GLFW and handle error
	if ( glfwInit() != GLFW_TRUE )
	{
		throw GlfwException{ tag, "Could not initialize GLFW" };
	}

	// Set the error callback
	glfwSetErrorCallback(
	    []( int error, const char* description ) { lst::Log::error( "%s: %s [%d]", tag.c_str(), description, error ); } );

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
	monitor_size.width  = videomode->width;
	monitor_size.height = videomode->height;

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
	handle = glfwCreateWindow( window.size.width, window.size.height, title.c_str(), nullptr, nullptr );
	if ( handle == nullptr )  // Handle window creation error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not create GLFW window" };
	}
	glfwSetWindowUserPointer( handle, this );
	glfwMakeContextCurrent( handle );
	glfwSwapInterval( 1 );  // Vsync

	// Hide the cursor and capture it, perfect for an FPS camera system
	// glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Listen to mouse events
	glfwSetMouseButtonCallback( handle, []( GLFWwindow* handle, int button, int act, int mods ) {
		auto win = reinterpret_cast<GlfwWindow*>( glfwGetWindowUserPointer( handle ) );
		win->handle_mouse( act );
	} );
	glfwSetCursorPosCallback( handle, []( GLFWwindow* handle, double xpos, double ypos ) {
		auto win = reinterpret_cast<GlfwWindow*>( glfwGetWindowUserPointer( handle ) );
		win->handle_mouse( xpos, ypos );
	} );

	// Listen to keyboard events
	glfwSetKeyCallback( handle, []( GLFWwindow* handle, int k, int /* scancode */, int act, int /* mode */ ) {
		auto win = reinterpret_cast<GlfwWindow*>( glfwGetWindowUserPointer( handle ) );
		win->handle_input( k, act );
	} );

	try  // Initialize glew
	{
		Window::init_glew();
	}
	catch ( const GlewException& )  // Handle error
	{
		glfwDestroyWindow( handle );
		glfwTerminate();
		throw;
	}

	update_size();

	lst::Log::info( "%s created\n\tOpenGL %s\n\tGLFW %s\n\tFrame size %dx%d\n", tag.c_str(), glGetString( GL_VERSION ),
	                glfwGetVersionString(), frame_size.width, frame_size.height );
}


GlfwWindow::~GlfwWindow()
{
	if ( handle != nullptr )
	{
		glfwDestroyWindow( handle );
	}
	glfwTerminate();
}


void GlfwWindow::set_closing( const bool closing ) const
{
	if ( closing )
	{
		glfwSetWindowShouldClose( handle, GLFW_TRUE );
	}
}

void GlfwWindow::handle_mouse( const int act )
{
	if ( act == GLFW_PRESS )
	{
		action = input::Action::PRESS;
	}
	else if ( act == GLFW_RELEASE )
	{
		action = input::Action::RELEASE;
	}
	Window::handle( input::Input{ type, key, action, position } );
}


void GlfwWindow::handle_mouse( const double x, const double y )  // TODO comment
{
	cursor.setPosition( static_cast<float>( x ), static_cast<float>( y ) );
	type       = input::Type::MOTION;
	position.x = static_cast<float>( x );
	position.y = static_cast<float>( y );
	Window::handle( input::Input{ type, key, action, position } );
}


const input::Input GlfwWindow::poll_input()
{
	glfwPollEvents();
	return input::Input{};
}


void GlfwWindow::handle_input( const int k, const int act )  // TODO comment
{
	if ( act == GLFW_PRESS )
	{
		action = input::Action::PRESS;
	}
	else if ( act == GLFW_RELEASE )
	{
		action = input::Action::RELEASE;
	}
	else if ( act == GLFW_REPEAT )
	{
		action = input::Action::REPEAT;
	}

	switch ( k )
	{
		case GLFW_KEY_A:
			key = input::Key::A;
			break;
		case GLFW_KEY_Q:
			key = input::Key::Q;
			break;
		case GLFW_KEY_W:
			key = input::Key::W;
			break;
		case GLFW_KEY_E:
			key = input::Key::E;
			break;
		case GLFW_KEY_S:
			key = input::Key::S;
			break;
		case GLFW_KEY_D:
			key = input::Key::D;
			break;
		case GLFW_KEY_UP:
			key = input::Key::UP;
			break;
		case GLFW_KEY_LEFT:
			key = input::Key::LEFT;
			break;
		case GLFW_KEY_RIGHT:
			key = input::Key::RIGHT;
			break;
		case GLFW_KEY_DOWN:
			key = input::Key::DOWN;
			break;
		case GLFW_KEY_F:
			key = input::Key::NONE;
			if ( act == GLFW_PRESS )
			{
				toggle_fullscreen();
			}
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose( handle, GLFW_TRUE );
			break;
		default:
			key = input::Key::NONE;
			break;
	}

	type = input::Type::KEY;
	Window::handle( input::Input{ type, key, action, position } );
}


void GlfwWindow::toggle_fullscreen()
{
	monitor   = glfwGetPrimaryMonitor();
	videomode = glfwGetVideoMode( monitor );
	if ( fullscreen )  // Use start-up values for windowed mode
	{
		glfwSetWindowMonitor( handle, nullptr,
		                      videomode->width / 2 - window.size.width / 2,    // X center
		                      videomode->height / 2 - window.size.height / 2,  // Y center
		                      window.size.width, window.size.height, videomode->refreshRate );
	}
	else  // Set window size for fullscreen-windowed mode to the desktop resolution
	{
		glfwSetWindowMonitor( handle, monitor,
		                      0,  // left
		                      0,  // top
		                      videomode->width, videomode->height, videomode->refreshRate );
	}
	glfwSwapInterval( 1 );  // Vsync
	fullscreen = !fullscreen;
}


void GlfwWindow::update_size()
{
	glfwGetFramebufferSize( handle, &frame_size.width, &frame_size.height );
	get_game().set_size( frame_size );
}


}  // namespace sunspot
