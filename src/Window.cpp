#include <iostream>

#include "Graphics.h"
#include "Window.h"
#include "Framebuffer.h"

Window::Window(const unsigned width, const unsigned height, const std::string &title)
	: window_{nullptr}
{
	glfwInit(); // Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // TODO refactor magic numbers
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

 	// Create a full screen window object
	window_ = glfwCreateWindow(width, height, title.c_str(), nullptr /*glfwGetPrimaryMonitor()*/, nullptr);
	if (window_ == nullptr) { // Handle error
		std::cerr << "Could not create GLFW window\n";
		glfwTerminate();
		exit(EXIT_FAILURE); // TODO refactor to exception
	}
	glfwMakeContextCurrent(window_);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { // Initialize GLEW and handle error
		std::cerr << "Could not initialize GLEW\n";
		exit(EXIT_FAILURE); // TODO refactor to exception
	}

	std::cout << "Window -> created\n"; // TODO remove debug log
}


Window::~Window()
{
	glfwTerminate();
	std::cout << "Window -> destroyed\n"; // TODO remove debug log
}


void Window::render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model& model)
{
	int width, height; // Dimensions from GLFW such that it also works on high DPI screens

	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();

		glfwGetFramebufferSize(window_, &width, &height);
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

		model.bind();
		
		glViewport(0, 0, width / 2, height);
		baseProgram.use();
		model.rotateY(0.0025f);
		model.rotateZ(0.0025f);
		//model.translateX(0.0125f);
		model.render(baseProgram);

		glViewport(width / 2, 0, width / 2, height);
		glDisable(GL_DEPTH_TEST);
		depthProgram.use();
		model.render(depthProgram);

		model.unbind();
		glfwSwapBuffers(window_);
	}
}


void Window::render(const ShaderProgram &program, const Quad& quad)
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


void Window::render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model &model,
			       const ShaderProgram &quadProgram, const Quad &quad)
{
	int width, height;
	glfwGetFramebufferSize(window_, &width, &height);
	Framebuffer frame {static_cast<unsigned>(width), static_cast<unsigned>(height / 2)};

	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();
		
		// First pass
		frame.bind();
		glViewport(0, 0, width, height / 2);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
		baseProgram.use();
		model.bind();
		
		glViewport(0, 0, width / 2, height / 2);
		baseProgram.use();
		model.render(baseProgram);

		glViewport(width / 2, 0, width / 2, height / 2);
		depthProgram.use();
		model.render(depthProgram);
		
		model.unbind();
		frame.unbind();

		// Second pass
		glViewport(0, 0, width, height);
		quadProgram.use();
		quadProgram.setUniforms();
		frame.bindColorTexture();
		frame.bindMaskTexture();
		frame.bindHeaderTexture();

		quad.bind();
		quad.render();
		quad.unbind();

		glfwSwapBuffers(window_);
	}
}
