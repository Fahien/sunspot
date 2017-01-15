#include <iostream>

#include "Graphics.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "WavefrontObject.h"
#include "Quad.h"
#include "Framebuffer.h"

using namespace sunspot;


const std::string Window::tag{ "Window" };


Window::Window(const char* title, const math::Size windowSize, const bool decorated, const bool stereoscopic)
	: title_{ title }
	, windowSize_{ windowSize }
	, monitorSize_{ windowSize }
	, frameSize_{ windowSize }
	, decorated_{ decorated }
	, stereoscopic_{ stereoscopic }
	, fullscreen_{ false }
	, currentTime_{ 0.0f }
	, lastTime_{ 0.0f }
	, deltaTime_{ 0.0f }
	, cursor_{}
	, camera_ { nullptr }
	, baseProgram_{ nullptr }
	, light_{ nullptr }
	, objs_{}
	, quadProgram_{ nullptr }
	, depthProgram_{ nullptr }
	, framebuffer_{ nullptr }
	, quad_{ nullptr }
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


void Window::render(const float &deltaTime) // TODO comment
{
	// std::cout << static_cast<int>(1.0f / deltaTime) << " "; // FPS
	updateFrameSize();
	if (stereoscopic_) { renderStereoscopic(deltaTime); }
	else { render3D(deltaTime); }
}


void Window::render3D(const float &deltaTime) // TODO comment
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, frameSize_.width, frameSize_.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
	baseProgram_->use();
	camera_->update(deltaTime, baseProgram_);
	light_->update(baseProgram_);
	for (WavefrontObject *obj : objs_) { obj->draw(baseProgram_); }
}


void Window::renderQuad(const float & /* deltaTime */)
{
	glViewport(0, 0, frameSize_.width, frameSize_.height);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color buffer
	quadProgram_->use();
	quad_->bind();
	quad_->render();
	quad_->unbind();
}


void Window::renderStereoscopic(const float &deltaTime)
{
	// First pass
	glEnable(GL_DEPTH_TEST);
	framebuffer_->bind(); // Render the scene on a framebuffer
	glViewport(0, 0, framebuffer_->getWidth(), framebuffer_->getHeight()); // Viewport for framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
	baseProgram_->use();
	camera_->update(deltaTime, baseProgram_);
	light_->update(baseProgram_);
	for (WavefrontObject *obj : objs_) { obj->draw(baseProgram_); }
	framebuffer_->unbind();

	// Second pass
	glDisable(GL_DEPTH_TEST);
	quad_->bind();
	glViewport(0, 0, frameSize_.width / 2, frameSize_.height);
	quadProgram_->use();
	framebuffer_->bindColorTexture(quadProgram_); // Render color on the left
	quad_->render();
	glViewport(frameSize_.width / 2, 0, frameSize_.width / 2, frameSize_.height);
	depthProgram_->use();
	framebuffer_->bindDepthTexture(depthProgram_); // Render depth on the right
	quad_->render();
	quad_->unbind();
}
