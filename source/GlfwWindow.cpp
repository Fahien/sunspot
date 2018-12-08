#include <iostream>

#include <Graphics.h>
#include <GlfwWindow.h>
#include <Camera.h>
#include <logspot/Logger.h>

using namespace sunspot;
namespace lst = logspot;


GlfwWindow::GlfwWindow(const char* title,
                       const mst::Size windowSize,
                       const bool decorated,
                       const bool stereoscopic)
:	Window::Window{ title, windowSize, decorated, stereoscopic }
,	mMonitor      { nullptr }
,	mVideoMode    { nullptr }
,	mWindow       { nullptr }
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
	mMonitor = glfwGetPrimaryMonitor();
	if (mMonitor == nullptr) // Handle error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not get primary monitor" };
	}

	// Get the video mode for the monitor
	mVideoMode = glfwGetVideoMode(mMonitor);
	if (mVideoMode == nullptr) // Handle error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not get video mode" };
	}
	mMonitorSize.width  = mVideoMode->width;
	mMonitorSize.height = mVideoMode->height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // TODO refactor magic numbers

	// Hard constraints
	glfwWindowHint(GLFW_DOUBLEBUFFER,         GL_TRUE);
	glfwWindowHint(GLFW_CLIENT_API,           GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE,       GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE,    GL_FALSE);
	glfwWindowHint(GLFW_DECORATED,    decorated ? GL_TRUE : GL_FALSE);
	glfwWindowHint(GLFW_FOCUSED,      GL_TRUE);
	glfwWindowHint(GLFW_RED_BITS,     mVideoMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS,   mVideoMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS,    mVideoMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mVideoMode->refreshRate);

	// Create a window object
	mWindow = glfwCreateWindow(mWindowSize.width, mWindowSize.height, mTitle, nullptr, nullptr);
	if (mWindow == nullptr) // Handle window creation error
	{
		glfwTerminate();
		throw GlfwException{ tag, "Could not create GLFW window" };
	}
	glfwSetWindowUserPointer(mWindow, this);
	glfwMakeContextCurrent(mWindow);

	// Hide the cursor and capture it, perfect for an FPS camera system
	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Listen to mouse events
	glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
	{
		auto win = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));
		win->handleMouse(action);
	});
	glfwSetCursorPosCallback(mWindow, [](GLFWwindow *window, double xpos, double ypos)
	{
		auto win = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));
		win->handleMouse(xpos, ypos);
	});

	// Listen to keyboard events
	glfwSetKeyCallback(mWindow, [](GLFWwindow *window, int key, int /* scancode */, int action, int /* mode */)
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
		glfwDestroyWindow(mWindow);
		glfwTerminate();
		throw;
	}

	updateFrameSize();
	glfwSwapInterval(1); // Vsync

	lst::Logger::log.Info("%s created\n\tOpenGL %s\n\tGLFW %s\n\tFrame size %dx%d\n",
	         tag.c_str(),
	         glGetString(GL_VERSION),
	         glfwGetVersionString(),
	         mFrameSize.width,
	         mFrameSize.height);
}


GlfwWindow::~GlfwWindow()
{
	if (mWindow != nullptr)
	{
		glfwDestroyWindow(mWindow);
	}
	glfwTerminate();
	lst::Logger::log.Info("%s: destroyed", tag.c_str()); // TODO remove debug log
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
		glfwSetWindowShouldClose(mWindow, GLFW_TRUE); break;
	  default:
		mKey = input::Key::NONE;
		break;
	}

	mType = input::Type::KEY;
	Window::handleInput(input::Input{ mType, mKey, mAction, mPosition });
}


void GlfwWindow::toggleFullscreen()
{
	mMonitor   = glfwGetPrimaryMonitor();
	mVideoMode = glfwGetVideoMode(mMonitor);
	if (mFullscreen) // Use start-up values for windowed mode
	{
		glfwSetWindowMonitor(mWindow,
		                     nullptr,
		                     mVideoMode->width  / 2 - mWindowSize.width  / 2, // X center
		                     mVideoMode->height / 2 - mWindowSize.height / 2, // Y center
		                     mWindowSize.width,
		                     mWindowSize.height,
		                     mVideoMode->refreshRate);
	}
	else // Set window size for fullscreen-windowed mode to the desktop resolution
	{
		glfwSetWindowMonitor(mWindow,
		                     mMonitor,
		                     0, // left
		                     0, // top
		                     mVideoMode->width,
		                     mVideoMode->height,
		                     mVideoMode->refreshRate);
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
	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();
		Window::render();
		glfwSwapBuffers(mWindow);
	}
}


const float& GlfwWindow::computeDeltaTime() // TODO comment
{
	mCurrentTime = static_cast<float>(glfwGetTime());
	mDeltaTime   = mCurrentTime - mLastTime;
	mLastTime    = mCurrentTime;
	return mDeltaTime;
}


void GlfwWindow::updateFrameSize()
{
	// Dimensions from GLFW such that it also works on high DPI screens
	glfwGetFramebufferSize(mWindow, &mFrameSize.width, &mFrameSize.height);
}
