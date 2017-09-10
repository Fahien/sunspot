#include <iostream>

#include "Graphics.h"
#include "GlfwWindow.h"
#include "Camera.h"

using namespace sunspot;


const Logger GlfwWindow::log{};

const std::string GlfwWindow::tag{ "GlfwWindow" };


GlfwWindow::GlfwWindow(const char* title,
                       const mst::Size windowSize,
                       const bool decorated,
                       const bool stereoscopic)
	: Window::Window{ title, windowSize, decorated, stereoscopic }
	, videoMode_    { nullptr }
	, window_       { nullptr }
{
	// Initialize GLFW and handle error
	if (glfwInit() != GLFW_TRUE)
		throw GlfwException{ tag, "Could not initialize GLFW" };
	// Set the error callback
	glfwSetErrorCallback([](int error, const char *description) {
		log.error("%s: %s [%d]\n", tag.c_str(), description, error);
	});

	// Get the primary monitor
	monitor_ = glfwGetPrimaryMonitor();
	if (monitor_ == nullptr) { // Handle error
		glfwTerminate();
		throw GlfwException{ tag, "Could not get primary monitor" };
	}

	// Get the video mode for the monitor
	videoMode_ = glfwGetVideoMode(monitor_);
	if (videoMode_ == nullptr) { // Handle error
		glfwTerminate();
		throw GlfwException{ tag, "Could not get video mode" };
	}
	monitorSize_.width  = videoMode_->width;
	monitorSize_.height = videoMode_->height;

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
	glfwWindowHint(GLFW_RED_BITS,     videoMode_->redBits);
	glfwWindowHint(GLFW_GREEN_BITS,   videoMode_->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS,    videoMode_->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, videoMode_->refreshRate);

	// Create a window object
	window_ = glfwCreateWindow(windowSize_.width, windowSize_.height, title_, nullptr, nullptr);
	if (window_ == nullptr) { // Handle window creation error
		glfwTerminate();
		throw GlfwException{ tag, "Could not create GLFW window" };
	}
	glfwSetWindowUserPointer(window_, this);
	glfwMakeContextCurrent(window_);

	// Hide the cursor and capture it, perfect for an FPS camera system
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Listen to mouse-movement events
	glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double xpos, double ypos) {
		GlfwWindow *win{ static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)) };
		win->handleMouse(xpos, ypos);
	});

	// Listen to keyboard events
	glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int /* scancode */, int action, int /* mode */) {
		GlfwWindow *win{ static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)) };
		win->handleInput(key, action);
	});

	try { // Initialize glew
		Window::initGlew();
	}
	catch (const GlewException &) { // Handle error
		glfwDestroyWindow(window_);
		glfwTerminate();
		throw;
	}

	updateFrameSize();
	glfwSwapInterval(1); // Vsync

	log.info("%s created\n\tOpenGL %s\n\tGLFW %s\n\tFrame size %dx%d\n",
	         tag.c_str(),
	         glGetString(GL_VERSION),
	         glfwGetVersionString(),
	         frameSize_.width,
	         frameSize_.height);
}


GlfwWindow::~GlfwWindow()
{
	if (window_ != nullptr)
		glfwDestroyWindow(window_);
	glfwTerminate();
	log.info("%s: destroyed\n", tag.c_str()); // TODO remove debug log
}


void GlfwWindow::handleMouse(const double x, const double y) // TODO comment
{
	cursor_.setPosition(static_cast<float>(x), static_cast<float>(y));
	camera_->setYaw(camera_->getYaw() - cursor_.getOffset().x * cursor_.getSensitivity() * deltaTime_);
	camera_->setPitch(camera_->getPitch() + cursor_.getOffset().y * cursor_.getSensitivity() * deltaTime_);
	camera_->updateVectors();
}


void GlfwWindow::handleInput(const int key, const int action) // TODO comment
{
	switch (key) {
	  case GLFW_KEY_W:
		if (action == GLFW_PRESS) { camera_->setVelocityZ(-1.0f); }
		else if (action == GLFW_RELEASE) { camera_->setVelocityZ(0.0f); }
		break;
	  case GLFW_KEY_S:
		if (action == GLFW_PRESS) { camera_->setVelocityZ(1.0f); }
		else if (action == GLFW_RELEASE) { camera_->setVelocityZ(0.0f); }
		break;
	  case GLFW_KEY_A:
		if (action == GLFW_PRESS) { camera_->setVelocityX(-1.0f); }
		else if(action == GLFW_RELEASE) { camera_->setVelocityX(0.0f); }
		break;
	  case GLFW_KEY_D:
		if (action == GLFW_PRESS) { camera_->setVelocityX(1.0f); }
		else if (action == GLFW_RELEASE) { camera_->setVelocityX(0.0f); }
		break;
	  case GLFW_KEY_F: if (action == GLFW_PRESS) { toggleFullscreen(); } break;
	  case GLFW_KEY_ESCAPE:
	  case GLFW_KEY_Q: glfwSetWindowShouldClose(window_, GLFW_TRUE); break;
	  default: break;
	}
}


void GlfwWindow::toggleFullscreen()
{
	monitor_ = glfwGetPrimaryMonitor();
	videoMode_ = glfwGetVideoMode(monitor_);
	if (fullscreen_) { // Use start-up values for windowed mode
		glfwSetWindowMonitor(window_,
		                     nullptr,
		                     videoMode_->width / 2 - windowSize_.width / 2, // X center
		                     videoMode_->height / 2 - windowSize_.height / 2, // Y center
		                     windowSize_.width,
		                     windowSize_.height,
		                     videoMode_->refreshRate);
	}
	else { // Set window size for fullscreen-windowed mode to the desktop resolution
		glfwSetWindowMonitor(window_,
		                     monitor_,
		                     0, // left
		                     0, // top
		                     videoMode_->width,
		                     videoMode_->height,
		                     videoMode_->refreshRate);
	}
	glfwSwapInterval(1); // Vsync
	fullscreen_ = !fullscreen_;
}


void GlfwWindow::loop() // TODO comment
{
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
#ifdef SST_PROFILING
	std::cout << "#Frame\tPass1\tPass2.1\tPass2.2\tTotal\tOverhead\n";
#endif
	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();
		Window::render();
		glfwSwapBuffers(window_);
	}
}


const float &GlfwWindow::computeDeltaTime() // TODO comment
{
	currentTime_ = static_cast<float>(glfwGetTime());
	deltaTime_   = currentTime_ - lastTime_;
	lastTime_    = currentTime_;
	return deltaTime_;
}


void GlfwWindow::updateFrameSize()
{
	// Dimensions from GLFW such that it also works on high DPI screens
	glfwGetFramebufferSize(window_, &frameSize_.width, &frameSize_.height);
}
