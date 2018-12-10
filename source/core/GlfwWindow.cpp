#include "sunspot/core/GlfwWindow.h"

#include <iostream>

#include <logspot/Logger.h>

#include "Camera.h"

using namespace sunspot;
namespace lst = logspot;


GlfwWindow::GlfwWindow(const char* title,
                       const mst::Size windowSize,
                       const bool decorated,
                       const bool stereoscopic)
:	Window::Window { title, windowSize, decorated, stereoscopic }
,	context { { 3, 3 }, GLFW_OPENGL_CORE_PROFILE }
,	m_Monitor      { nullptr }
,	m_VideoMode    { nullptr }
,	m_Window       { nullptr }
{
	// Initialize GLFW and handle error
	if (glfwInit() != GLFW_TRUE)
	{
		throw GlfwException{ tag, "Could not initialize GLFW" };
	}
	// Set the error callback
	glfwSetErrorCallback([](int error, const char *description)
	{
		lst::Logger::log.Error("%s: %s [%d]", tag.c_str(), description, error);
	});

	// Get the primary monitor
	m_Monitor = glfwGetPrimaryMonitor();
	if (m_Monitor == nullptr) // Handle error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not get primary monitor" };
	}

	// Get the video mode for the monitor
	m_VideoMode = glfwGetVideoMode(m_Monitor);
	if (m_VideoMode == nullptr) // Handle error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not get video mode" };
	}
	m_MonitorSize.width  = m_VideoMode->width;
	m_MonitorSize.height = m_VideoMode->height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context.version.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context.version.minor);

	// Hard constraints
	glfwWindowHint(GLFW_DOUBLEBUFFER,         GL_TRUE);
	glfwWindowHint(GLFW_CLIENT_API,           GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE,       context.profile);

	glfwWindowHint(GLFW_RESIZABLE,    GL_FALSE);
	glfwWindowHint(GLFW_DECORATED,    decorated ? GL_TRUE : GL_FALSE);
	glfwWindowHint(GLFW_FOCUSED,      GL_TRUE);
	glfwWindowHint(GLFW_RED_BITS,     m_VideoMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS,   m_VideoMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS,    m_VideoMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, m_VideoMode->refreshRate);

	// Create a window object
	m_Window = glfwCreateWindow(m_WindowSize.width, m_WindowSize.height, m_Title, nullptr, nullptr);
	if (m_Window == nullptr) // Handle window creation error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not create GLFW window" };
	}
	glfwSetWindowUserPointer(m_Window, this);
	glfwMakeContextCurrent(m_Window);

	// Hide the cursor and capture it, perfect for an FPS camera system
	//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Listen to mouse events
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
	{
		auto win = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));
		win->handleMouse(action);
	});
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xpos, double ypos)
	{
		auto win = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));
		win->handleMouse(xpos, ypos);
	});

	// Listen to keyboard events
	glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int /* scancode */, int action, int /* mode */)
	{
		auto*win = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));
		win->handleInput(key, action);
	});

	try // Initialize glew
	{
		Window::initGlew();
	}
	catch (const GlewException &) // Handle error
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		throw;
	}

	UpdateSize();
	glfwSwapInterval(1); // Vsync

	lst::Logger::log.Info("%s created\n\tOpenGL %s\n\tGLFW %s\n\tFrame size %dx%d\n",
	         tag.c_str(),
	         glGetString(GL_VERSION),
	         glfwGetVersionString(),
	         m_FrameSize.width,
	         m_FrameSize.height);
}


GlfwWindow::~GlfwWindow()
{
	if (m_Window != nullptr)
	{
		glfwDestroyWindow(m_Window);
	}
	glfwTerminate();
	lst::Logger::log.Info("%s: destroyed", tag.c_str()); // TODO remove debug log
}


std::string GlfwWindow::GetGlslVersion() const
{
	std::string ret = "#version ";

	ret += context.version.major;
	ret += context.version.minor;
	ret += "0";

	if ( context.profile == GLFW_OPENGL_CORE_PROFILE )
	{
		ret += " core";
	}

	return ret;
}

void GlfwWindow::handleMouse(const int action)
{
	if (action == GLFW_PRESS)
	{
		mAction = input::Action::PRESS;
	}
	else if (action == GLFW_RELEASE)
	{
		mAction = input::Action::RELEASE;
	}
	Window::handleInput(input::Input{ mType, mKey, mAction, mPosition });
}


void GlfwWindow::handleMouse(const double x, const double y) // TODO comment
{
	mCursor.setPosition(static_cast<float>(x), static_cast<float>(y));
	mType = input::Type::MOTION;
	mPosition.x = static_cast<float>(x);
	mPosition.y = static_cast<float>(y);
	Window::handleInput(input::Input{ mType, mKey, mAction, mPosition });
	//mCamera->setYaw  (mCamera->getYaw()   - mCursor.getOffset().x * mCursor.getSensitivity() * mDeltaTime);
	//mCamera->setPitch(mCamera->getPitch() + mCursor.getOffset().y * mCursor.getSensitivity() * mDeltaTime);
	//mCamera->updateVectors();
}


const input::Input GlfwWindow::pollInput()
{
	glfwPollEvents();
	return input::Input{};
}


void GlfwWindow::handleInput(const int key, const int action) // TODO comment
{
	if (action == GLFW_PRESS)
	{
		mAction = input::Action::PRESS;
	}
	else if (action == GLFW_RELEASE)
	{
		mAction = input::Action::RELEASE;
	}
	else if (action == GLFW_REPEAT)
	{
		mAction = input::Action::REPEAT;
	}

	switch (key)
	{
	  case GLFW_KEY_A: mKey = input::Key::A; break;
	  case GLFW_KEY_Q: mKey = input::Key::Q; break;
	  case GLFW_KEY_W: mKey = input::Key::W; break;
	  case GLFW_KEY_E: mKey = input::Key::E; break;
	  case GLFW_KEY_S: mKey = input::Key::S; break;
	  case GLFW_KEY_D: mKey = input::Key::D; break;
	  case GLFW_KEY_UP: mKey = input::Key::UP; break;
	  case GLFW_KEY_LEFT: mKey = input::Key::LEFT; break;
	  case GLFW_KEY_RIGHT: mKey = input::Key::RIGHT; break;
	  case GLFW_KEY_DOWN: mKey = input::Key::DOWN; break;
	  case GLFW_KEY_F:
		mKey = input::Key::NONE;
		if (action == GLFW_PRESS)
		{
			toggleFullscreen();
		}
		break;
	  case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(m_Window, GLFW_TRUE); break;
	  default:
		mKey = input::Key::NONE;
		break;
	}

	mType = input::Type::KEY;
	Window::handleInput(input::Input{ mType, mKey, mAction, mPosition });
}


void GlfwWindow::toggleFullscreen()
{
	m_Monitor   = glfwGetPrimaryMonitor();
	m_VideoMode = glfwGetVideoMode(m_Monitor);
	if (mFullscreen) // Use start-up values for windowed mode
	{
		glfwSetWindowMonitor(m_Window,
		                     nullptr,
		                     m_VideoMode->width  / 2 - m_WindowSize.width  / 2, // X center
		                     m_VideoMode->height / 2 - m_WindowSize.height / 2, // Y center
		                     m_WindowSize.width,
		                     m_WindowSize.height,
		                     m_VideoMode->refreshRate);
	}
	else // Set window size for fullscreen-windowed mode to the desktop resolution
	{
		glfwSetWindowMonitor(m_Window,
		                     m_Monitor,
		                     0, // left
		                     0, // top
		                     m_VideoMode->width,
		                     m_VideoMode->height,
		                     m_VideoMode->refreshRate);
	}
	glfwSwapInterval(1); // Vsync
	mFullscreen = !mFullscreen;
}


void GlfwWindow::loop() // TODO comment
{
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
#ifdef SST_PROFILING
	std::cout << "#Frame\tPass1\tPass2.1\tPass2.2\tTotal\tOverhead\n";
#endif
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwPollEvents();
		Window::render();
		glfwSwapBuffers(m_Window);
	}
}



void GlfwWindow::UpdateSize()
{
	glfwGetFramebufferSize( m_Window, &m_FrameSize.width, &m_FrameSize.height );
}
