#include <iostream>

#include "Graphics.h"
#include "GlfwWindow.h"
#include "Framebuffer.h"
#include "Camera.h"

GlfwWindow::GlfwWindow(const unsigned width, const unsigned height, const std::string &title)
	: Window::Window{ width, height }
	, window_ {nullptr}
{
	glfwInit(); // Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a full screen window object
	window_ = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	if (window_ == nullptr) { // Handle error
		std::cerr << "Could not create GLFW window\n";
		glfwTerminate();
		exit(EXIT_FAILURE); // TODO refactor to exception
	}
	glfwMakeContextCurrent(window_);

	glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mode) {

	});

	Window::initGlew();
	std::cout << "GlfwWindow -> created\n"; // TODO remove debug log
}


GlfwWindow::~GlfwWindow()
{
	glfwTerminate();
	std::cout << "GlfwWindow -> destroyed\n"; // TODO remove debug log
}


void GlfwWindow::render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model& model)
{
	int width, height; // Dimensions from GLFW such that it also works on high DPI screens
	glfwGetFramebufferSize(window_, &width, &height);
	Camera camera{ 45.0f, width / 2.0f / height, 0.125f, 8.0f };

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

		glfwGetFramebufferSize(window_, &width, &height);
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

		model.bind();

		glViewport(0, 0, width / 2, height);
		baseProgram.use();
		camera.update(baseProgram);
		model.rotateY(deltaTime);
		model.render(baseProgram);

		glViewport(width / 2, 0, width / 2, height);
		glDisable(GL_DEPTH_TEST);
		depthProgram.use();
		camera.update(depthProgram);
		model.render(depthProgram);

		model.unbind();
		glfwSwapBuffers(window_);
	}
}


void GlfwWindow::render(const ShaderProgram &program, const Quad& quad)
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
	int width, height;
	glfwGetFramebufferSize(window_, &width, &height);
	Framebuffer frame{ static_cast<unsigned>(width), static_cast<unsigned>(height / 2) };
	float aspectRatio{ (width / 2.0f) / (height / 2.0f) };
	Camera camera{ 45.0f, aspectRatio, 0.125f, 8.0f };

	glfwSwapInterval(1); // Vsync

	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents(); // Processes events that have already been received

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