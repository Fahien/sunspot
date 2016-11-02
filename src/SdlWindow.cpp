#include <iostream>

#include "SdlWindow.h"
#include "Camera.h"


SdlWindow::SdlWindow(const unsigned width, const unsigned height, const std::string &title)
	: Window::Window {width, height}
	, window_{ nullptr }
	, context_{ nullptr }
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { // Initialize SDL video subsystem
		std::cerr << "SdlWindow -> Could not initialize video subsystem: " << SDL_GetError() << std::endl; // TODO throw an exception
	}
	window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (window_ == nullptr) { // TODO throw an exception
		std::cerr << "SdlWindow -> Could not create window: " << SDL_GetError() << std::endl;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // Request OpenGL 3.3 context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Turn on double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Set 16 bit Z buffer
	context_ = SDL_GL_CreateContext(window_);
	if (context_ == nullptr) { // TODO throw an exception
		std::cerr << "SdlWindow -> Could not create OpenGL Context: " << SDL_GetError() << std::endl;
	}
	SDL_GL_SetSwapInterval(1); // Syncronize with the monitor's vertical refresh

	Window::initGlew();
	std::cout << "SdlWindow -> created\n"; // TODO remove debug log
}


SdlWindow::~SdlWindow()
{
	if (context_ != nullptr) { SDL_GL_DeleteContext(context_); }
	if (window_ != nullptr) { SDL_DestroyWindow(window_); }
	SDL_Quit();
	std::cout << "SdlWindow -> destroyed\n"; // TODO remove debug log
}


void SdlWindow::render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model& model)
{
	Camera camera{ 45.0f, width_ / 2.0f / height_, 0.125f, 8.0f };

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glViewport(0, 0, width_, height_);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

	model.bind();

	glViewport(0, 0, width_ / 2, height_);
	baseProgram.use();
	camera.update(baseProgram);
	model.rotateY(0.0025f);
	model.render(baseProgram);

	glViewport(width_ / 2, 0, width_ / 2, height_);
	glDisable(GL_DEPTH_TEST);
	depthProgram.use();
	camera.update(depthProgram);
	model.render(depthProgram);

	model.unbind();

	SDL_GL_SwapWindow(window_); // Swap back buffer to the front
	SDL_Delay(2000);
}