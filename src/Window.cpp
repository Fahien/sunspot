#include <iostream>

#include "Graphics.h"
#include "Window.h"


const std::string Window::tag{ "Window" };


Window::Window(const unsigned width, const unsigned height, const char* title)
	: width_{ width }
	, height_{ height }
	, title_{ title }
	, monitorWidth_{ width }
	, monitorHeight_{ height }
	, currentTime_{ 0.0f }
	, lastTime_{ 0.0f }
	, deltaTime_{ 0.0f }
	, baseProgram_{ nullptr }
	, depthProgram_{ nullptr }
	, model_{ nullptr }
	, quad_{ nullptr }
	, camera_ { nullptr }
	, framebuffer_{ nullptr }
	, fullscreen_{ false }
{}


void Window::initGlew()
{
	glewExperimental = GL_TRUE; // Initialize GLEW and handle error
	if (glewInit() != GLEW_OK) { throw GlewException(tag); }
}


void Window::render()
{
	render(computeDeltaTime());
}