#include <iostream>

#include "Graphics.h"
#include "GlfwWindow.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Quad.h"
#include "Framebuffer.h"
#include "Camera.h"


const std::string GlfwWindow::tag{ "GlfwWindow" };


GlfwWindow::GlfwWindow(const unsigned width, const unsigned height, const char *title)
	: Window::Window{ width, height, title }
	, rotateY_{ false }
	, window_{ nullptr }
	, videoMode_{ nullptr }
{
	// Initialize GLFW and handle error
	if (glfwInit() != GLFW_TRUE) { throw GlfwException{ tag, "Could not initialize GLFW" }; }
	// Set the error callback
	glfwSetErrorCallback([](int error, const char *description) {
		std::cerr << tag << ": " << description << " [" << error << "]\n";
	});

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Create a full screen window object
	window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window_ == nullptr) { // Handle window creation error
		glfwTerminate();
		throw GlfwException{ tag, "Could not create GLFW window" };
	}
	glfwSetWindowUserPointer(window_, this);
	glfwMakeContextCurrent(window_);
	glfwSwapInterval(1); // Vsync

	glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
		GlfwWindow *win{ static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)) };
		win->handleInput(key, action);
	});

	const GLFWvidmode *videoMode_{ glfwGetVideoMode(glfwGetPrimaryMonitor()) }; // Get the monitor resolution.
	if (videoMode_ == nullptr) { // Handle error
		glfwDestroyWindow(window_);
		glfwTerminate();
		throw GlfwException{ tag, "Could not get video mode" };
	}
	monitorWidth_ = videoMode_->width;
	monitorHeight_ = videoMode_->height;

	try { Window::initGlew(); }
	catch (GlewException &) {
		glfwDestroyWindow(window_);
		glfwTerminate();
		throw;
	}

	std::cout << tag << ": created\n\tOpenGL " << glGetString(GL_VERSION) << "\n\tGLFW " << glfwGetVersionString() << std::endl;
}


GlfwWindow::~GlfwWindow()
{
	if (window_ != nullptr) { glfwDestroyWindow(window_); }
	glfwTerminate();
	std::cout << tag << ": destroyed\n"; // TODO remove debug log
}


void GlfwWindow::handleInput(const int key, const int action)
{
	switch (key) {
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
	if (fullscreen_) { // Use start-up values for "windowed" mode
		GLFWmonitor *monitor{ glfwGetPrimaryMonitor() };
		videoMode_ = glfwGetVideoMode(monitor);
		unsigned centerX{ videoMode_->width / 2 - width_ / 2 };
		unsigned centerY{ videoMode_->height / 2 - height_ / 2 };
		glfwSetWindowMonitor(window_, nullptr, centerX, centerY, width_, height_, videoMode_->refreshRate);
		fullscreen_ = false;
	}
	else { // Set window size for "fullscreen windowed" mode to the desktop resolution
		GLFWmonitor *monitor{ glfwGetPrimaryMonitor() };
		videoMode_ = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window_, monitor, 0, 0, videoMode_->width, videoMode_->height, videoMode_->refreshRate);
		fullscreen_ = true;
	}
}


void GlfwWindow::loop()
{
	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();
		Window::render();
		glfwSwapBuffers(window_);
	}
}


const float &GlfwWindow::computeDeltaTime()
{
	currentTime_ = static_cast<float>(glfwGetTime());
	deltaTime_ = currentTime_ - lastTime_;
	lastTime_ = currentTime_;
	return deltaTime_;
}


void GlfwWindow::render(const float &deltaTime) const
{
	render3D(deltaTime);
}


void GlfwWindow::render3D(const float &deltaTime) const
{
	float rotationVelocity = 0.25f;
	glEnable(GL_DEPTH_TEST);
	baseProgram_->use();
	glViewport(0, 0, width_, height_);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

	model_->bind();
	camera_->update(deltaTime, baseProgram_);
	model_->transform.rotateX(deltaTime * rotationVelocity);
	model_->transform.rotateY(deltaTime * rotationVelocity);
	model_->transform.rotateZ(deltaTime * rotationVelocity);
	model_->render(baseProgram_);
	model_->unbind();
}


void GlfwWindow::render3DplusDepth(const float& deltaTime) const
{
	glViewport(0, 0, width_, height_);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

	model_->bind();

	glViewport(0, 0, width_ / 2, height_);
	baseProgram_->use();
	camera_->update(deltaTime, baseProgram_);
	model_->transform.rotateY(deltaTime);
	model_->render(baseProgram_);

	glViewport(width_ / 2, 0, width_ / 2, height_);
	glDisable(GL_DEPTH_TEST);
	depthProgram_->use();
	camera_->update(deltaTime, depthProgram_);
	model_->render(depthProgram_);

	model_->unbind();
}


void GlfwWindow::renderQuad(const float &deltaTime) const
{
	int width, height; // Dimensions from GLFW such that it also works on high DPI screens
	glfwGetFramebufferSize(window_, &width, &height);

	quadProgram_->use();
	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

	quad_->bind();
	quad_->render();
	quad_->unbind();
}

void GlfwWindow::renderStereoscopic(const float &deltaTime) const
{
	int width, height; // Dimensions from GLFW such that it also works on high DPI screens
	glfwGetFramebufferSize(window_, &width, &height);

	framebuffer_->bind(); // First pass: render the scene on a framebuffer
	glViewport(0, 0, width, height / 2); // Viewport for color and depth sub-images
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer

	if (rotateY_) { model_->transform.rotateY(deltaTime); }
	model_->bind();
	baseProgram_->use();
	camera_->update(deltaTime, baseProgram_);
	glViewport(0, 0, width / 2, height / 2); // Render color sub-image
	model_->render(baseProgram_);
	glViewport(width / 2, 0, width / 2, height / 2); // Render depth sub-image
	depthProgram_->use();
	camera_->update(deltaTime, depthProgram_);
	model_->render(depthProgram_);
	model_->unbind();
	framebuffer_->unbind(); // End first pass

	glViewport(0, 0, width, height); // Second pass: render the framebuffer on a quad
	glDisable(GL_DEPTH_TEST);
	quadProgram_->use();
	quadProgram_->setUniforms();
	framebuffer_->bindColorTexture();
	framebuffer_->bindMaskTexture();
	framebuffer_->bindHeaderTexture();

	quad_->bind();
	quad_->render();
	quad_->unbind(); // End second pass
}