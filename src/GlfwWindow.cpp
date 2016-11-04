#include <iostream>

#include "Graphics.h"
#include "GlfwWindow.h"
#include "Framebuffer.h"
#include "Camera.h"


const std::string GlfwWindow::tag{ "GlfwWindow" };


GlfwWindow::GlfwWindow(const unsigned width, const unsigned height, const std::string &title)
	: Window::Window{ width, height }
	, window_{nullptr}
	, keyCallback_{}
	, videoMode_{nullptr}
{
	if (glfwInit() != GLFW_TRUE) { // Initialize GLFW and handle error
		throw GlfwException{ tag, "Could not initialize GLFW" };
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a full screen window object
	window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window_ == nullptr) { // Handle error
		glfwTerminate();
		throw GlfwException{ tag, "Could not create GLFW window" };
	}
	glfwSetWindowUserPointer(window_, this);
	glfwMakeContextCurrent(window_);
	glfwSwapInterval(1); // Vsync

	glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
		GlfwWindow *win{ static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)) };
		if (action != GLFW_PRESS) win->handleInput(key);
	});

	const GLFWvidmode *videoMode_{ glfwGetVideoMode(glfwGetPrimaryMonitor()) }; // Get the monitor resolution.
	if (videoMode_ == nullptr) { // Handle error
		glfwDestroyWindow(window_);
		glfwTerminate();
		throw GlfwException{ tag, "Could not get video mode" };
	}
	monitorWidth_ = videoMode_->width;
	monitorHeight_ = videoMode_->height;

	try {
		Window::initGlew();
	}
	catch (GlewException &) {
		glfwDestroyWindow(window_);
		glfwTerminate();
		throw;
	}
	std::cout << tag << ": created\n"; // TODO remove debug log
}


GlfwWindow::~GlfwWindow()
{
	if (window_ != nullptr) { glfwDestroyWindow(window_); }
	glfwTerminate();
	std::cout << tag << ": destroyed\n"; // TODO remove debug log
}


void GlfwWindow::handleInput(int key)
{
	switch (key) {
	case GLFW_KEY_F:
		toggleFullscreen();
		break;
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window_, GLFW_TRUE);
		break;
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


void GlfwWindow::render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model& model)
{
	Camera camera{ 45.0f, width_ / 2.0f / height_, 0.125f, 8.0f };

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float lastFrame = 0.0f;
	float currentFrame = static_cast<float>(glfwGetTime());
	float deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();

		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glViewport(0, 0, width_, height_);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

		model.bind();

		glViewport(0, 0, width_ / 2, height_);
		baseProgram.use();
		camera.update(baseProgram);
		model.rotateY(deltaTime);
		model.render(baseProgram);

		glViewport(width_ / 2, 0, width_ / 2, height_);
		glDisable(GL_DEPTH_TEST);
		depthProgram.use();
		camera.update(depthProgram);
		model.render(depthProgram);

		model.unbind();
		glfwSwapBuffers(window_);
	}
}


void GlfwWindow::render(const ShaderProgram &program, const Quad &quad)
{
	int width, height; // Dimensions from GLFW such that it also works on high DPI screens
	program.use();

	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();

		glfwGetFramebufferSize(window_, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

		quad.bind();
		quad.render();
		quad.unbind();

		glfwSwapBuffers(window_);
	}
}


void GlfwWindow::render(const ShaderProgram &modelProgram, const ShaderProgram &depthProgram, Model &model,
	const ShaderProgram &quadProgram, const Quad &quad)
{
	int width = width_;
	int height = height_;
	//glfwGetFramebufferSize(window_, &width, &height);
	Framebuffer frame{ static_cast<unsigned>(width), static_cast<unsigned>(height / 2) };
	float aspectRatio{ (width / 2.0f) / (height / 2.0f) };
	Camera camera{ 45.0f, aspectRatio, 0.125f, 8.0f };

	glfwSwapInterval(1); // Vsync

	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents(); // Processes events that have already been received
		glfwGetFramebufferSize(window_, &width, &height);

		frame.bind(); // First pass: render the scene on a framebuffer
		glViewport(0, 0, width, height / 2); // Viewport for color and depth sub-images
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer

		model.bind();
		modelProgram.use();
		camera.update(modelProgram);
		glViewport(0, 0, width / 2, height / 2); // Render color sub-image
		model.rotateY(0.0025f);
		model.render(modelProgram);
		glViewport(width / 2, 0, width / 2, height / 2); // Render depth sub-image
		depthProgram.use();
		camera.update(depthProgram);
		model.render(depthProgram);
		model.unbind();
		frame.unbind(); // End first pass

		glViewport(0, 0, width, height); // Second pass: render the framebuffer on a quad
		glDisable(GL_DEPTH_TEST);
		quadProgram.use();
		quadProgram.setUniforms();
		frame.bindColorTexture();
		frame.bindMaskTexture();
		frame.bindHeaderTexture();

		quad.bind();
		quad.render();
		quad.unbind(); // End second pass

		glfwSwapBuffers(window_);
	}
}