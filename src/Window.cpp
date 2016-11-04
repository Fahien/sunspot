#include <iostream>

#include "Graphics.h"
#include "Window.h"


const std::string Window::tag{ "Window" };


Window::Window(const unsigned width, const unsigned height)
	: width_ {width}
	, height_ {height}
	, monitorWidth_{ width }
	, monitorHeight_{ height }
	, fullscreen_ {false}
{}


void Window::initGlew()
{
	glewExperimental = GL_TRUE; // Initialize GLEW and handle error
	if (glewInit() != GLEW_OK) { throw GlewException(tag); }
}