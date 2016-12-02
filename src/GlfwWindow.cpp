#include <iostream>

#include "Graphics.h"
#include "GlfwWindow.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "Model.h"
#include "Mesh.h"
#include "Quad.h"
#include "Framebuffer.h"
#include "Camera.h"

using namespace sunspot;


const std::string GlfwWindow::tag{ "GlfwWindow" };


GlfwWindow::GlfwWindow(const char *title, const int width, const int height, const bool stereo, const bool decorated)
	: Window::Window{title, width, height}
	, rotateY_{ false }
	, monitor_{ nullptr }
	, videoMode_{ nullptr }
	, window_{ nullptr }
	, cursor_{}
{
	// Initialize GLFW and handle error
	if (glfwInit() != GLFW_TRUE) { throw GlfwException{ tag, "Could not initialize GLFW" }; }
	// Set the error callback
	glfwSetErrorCallback([](int error, const char *description) {
		std::cerr << tag << ": " << description << " [" << error << "]\n";
	});

	monitor_ = glfwGetPrimaryMonitor(); // Get the primary monitor
	if (monitor_ == nullptr) { // Handle error
		glfwTerminate();
		throw GlfwException{ tag, "Could not get primary monitor" };
	}

	videoMode_ = glfwGetVideoMode(monitor_); // Get the video mode for the monitor
	if (videoMode_ == nullptr) { // Handle error
		glfwTerminate();
		throw GlfwException{ tag, "Could not get video mode" };
	}
	monitorSize_.width = videoMode_->width;
	monitorSize_.height = videoMode_->height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // TODO refactor magic numbers

	// Hard constraints
	glfwWindowHint(GLFW_STEREO, stereo ? GL_TRUE : GL_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DECORATED, decorated ? GL_TRUE : GL_FALSE);
	glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
	glfwWindowHint(GLFW_RED_BITS, videoMode_->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, videoMode_->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, videoMode_->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, videoMode_->refreshRate);
	// Create a window object
	window_ = glfwCreateWindow(videoMode_->width, videoMode_->height, title, monitor_, nullptr);
	if (window_ == nullptr) { // Handle window creation error
		glfwTerminate();
		throw GlfwException{ tag, "Could not create GLFW window" };
	}
	glfwSetWindowUserPointer(window_, this);
	glfwMakeContextCurrent(window_);

	// Hide the cursor and capture it, perfect for an FPS camera system
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Listen to mouse-movement events
	glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos) {
		GlfwWindow *win{ static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)) };
		win->handleMouse(xpos, ypos);
	});

	// Listen to keyboard events
	glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
		GlfwWindow *win{ static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)) };
		win->handleInput(key, action);
	});

	try { Window::initGlew(); } // Initialize glew
	catch (GlewException &) { // Handle error
		glfwDestroyWindow(window_);
		glfwTerminate();
		throw;
	}
	glfwGetFramebufferSize(window_, &frameSize_.width, &frameSize_.height);
	glfwSwapInterval(1); // Vsync

	std::cout << tag << ": created\n\tOpenGL " << glGetString(GL_VERSION) << "\n\tGLFW " << glfwGetVersionString() << std::endl;
}


GlfwWindow::~GlfwWindow()
{
	if (window_ != nullptr) { glfwDestroyWindow(window_); }
	glfwTerminate();
	std::cout << tag << ": destroyed\n"; // TODO remove debug log
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
	  case GLFW_KEY_Y: rotateY_ = !rotateY_; break;
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
	if (fullscreen_) { // Use start-up values for "windowed" mode
		glfwSetWindowMonitor(window_, nullptr,
			videoMode_->width / 2 - windowSize_.width / 2, // X center
			videoMode_->height / 2 - windowSize_.height / 2, // Y center
			windowSize_.width, windowSize_.height, 0);
	}
	else { // Set window size for "fullscreen windowed" mode to the desktop resolution
		glfwSetWindowMonitor(window_, monitor_, 0, 0, videoMode_->width, videoMode_->height, videoMode_->refreshRate);
	}
	glfwSwapInterval(1); // Vsync
	fullscreen_ = !fullscreen_;
}


void GlfwWindow::loop() // TODO comment
{
	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();
		Window::render();
		glfwSwapBuffers(window_);
	}
}


const float &GlfwWindow::computeDeltaTime() // TODO comment
{
	currentTime_ = static_cast<float>(glfwGetTime());
	deltaTime_ = currentTime_ - lastTime_;
	lastTime_ = currentTime_;
	return deltaTime_;
}


void GlfwWindow::render(const float &deltaTime) // TODO comment
{
	// std::cout << static_cast<int>(1.0f / deltaTime) << " ";
	renderMesh(deltaTime);
}


void GlfwWindow::renderMesh(const float &deltaTime) // TODO comment
{
	glfwGetFramebufferSize(window_, &frameSize_.width, &frameSize_.height);

	glEnable(GL_DEPTH_TEST);
	baseProgram_->use();
	glViewport(0, 0, frameSize_.width, frameSize_.height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera_->update(deltaTime, baseProgram_);
	light_->update(baseProgram_);
	room_->bind();
	room_->render(baseProgram_);
	room_->unbind();
	mesh_->draw(baseProgram_);
}


void GlfwWindow::render3D(const float &deltaTime) // TODO comment
{
	glfwGetFramebufferSize(window_, &frameSize_.width, &frameSize_.height);

	float rotationVelocity{ 0.25f };
	glEnable(GL_DEPTH_TEST);
	baseProgram_->use();
	glViewport(0, 0, frameSize_.width, frameSize_.height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer

	camera_->update(deltaTime, baseProgram_);
	light_->update(baseProgram_);
	room_->bind();
	room_->render(baseProgram_);
	room_->unbind();
	model_->bind();
	if (rotateY_) {
		model_->transform.rotateX(deltaTime * rotationVelocity);
		model_->transform.rotateY(deltaTime * rotationVelocity);
		model_->transform.rotateZ(deltaTime * rotationVelocity);
	}
	model_->render(baseProgram_);
	model_->unbind();
}


void GlfwWindow::render3DplusDepth(const float& deltaTime) // TODO comment
{
	glViewport(0, 0, frameSize_.width, frameSize_.height);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

	model_->bind();

	glViewport(0, 0, frameSize_.width / 2, frameSize_.height);
	baseProgram_->use();
	camera_->update(deltaTime, baseProgram_);
	light_->update(baseProgram_);
	if (rotateY_) model_->transform.rotateY(deltaTime);
	model_->render(baseProgram_);

	glViewport(frameSize_.width / 2, 0, frameSize_.width / 2, frameSize_.height);
	depthProgram_->use();
	camera_->update(deltaTime, depthProgram_);
	model_->render(depthProgram_);

	model_->unbind();
}


void GlfwWindow::renderQuad(const float &deltaTime)
{
	glfwGetFramebufferSize(window_, &frameSize_.width, &frameSize_.height);

	quadProgram_->use();
	glViewport(0, 0, frameSize_.width, frameSize_.height);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

	quad_->bind();
	quad_->render();
	quad_->unbind();
}

void GlfwWindow::renderStereoscopic(const float &deltaTime)
{
	// Dimensions from GLFW such that it also works on high DPI screens
	glfwGetFramebufferSize(window_, &frameSize_.width, &frameSize_.height);

	framebuffer_->bind(); // First pass: render the scene on a framebuffer
	glViewport(0, 0, frameSize_.width, frameSize_.height / 2); // Viewport for color and depth sub-images
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer

	if (rotateY_) { model_->transform.rotateY(deltaTime); }
	model_->bind();
	baseProgram_->use();
	camera_->update(deltaTime, baseProgram_);
	light_->update(baseProgram_);
	glViewport(0, 0, frameSize_.width / 2, frameSize_.height / 2); // Render color sub-image
	model_->render(baseProgram_);
	glViewport(frameSize_.width / 2, 0, frameSize_.width / 2, frameSize_.height / 2); // Render depth sub-image
	depthProgram_->use();
	camera_->update(deltaTime, depthProgram_);
	model_->render(depthProgram_);
	model_->unbind();
	framebuffer_->unbind(); // End first pass

	glViewport(0, 0, frameSize_.width, frameSize_.height); // Second pass: render the framebuffer on a quad
	glDisable(GL_DEPTH_TEST);
	quadProgram_->use();
	framebuffer_->bindTextures(quadProgram_);

	quad_->bind();
	quad_->render();
	quad_->unbind(); // End second pass
}

