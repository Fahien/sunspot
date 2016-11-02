#include <iostream>

#include "Graphics.h"
#include "Window.h"

Window::Window(const unsigned width, const unsigned height)
	: width_ {width}
	, height_ {height}
{}

void Window::initGlew()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { // Initialize GLEW and handle error
		std::cerr << "Could not initialize GLEW\n";
		exit(EXIT_FAILURE); // TODO refactor to exception
	}
}