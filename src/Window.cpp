#include <iostream>

#include "Config.h"
#ifdef SST_PROFILING
#include <chrono>
#endif

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


Window::Window(const char* title, const mst::Size windowSize, const bool decorated, const bool stereoscopic)
	: title_{ title }
	, windowSize_{ windowSize }
	, monitorSize_{ windowSize }
	, frameSize_{ windowSize }
	, decorated_{ decorated }
	, stereoscopic_{ stereoscopic }
	, fullscreen_{ true }
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
	camera_->update(deltaTime, *baseProgram_);
	light_->update(baseProgram_);
	for (WavefrontObject *obj : objs_) { obj->draw(*baseProgram_); }
}


void Window::renderQuad(const float & /* deltaTime */)
{
	glViewport(0, 0, frameSize_.width, frameSize_.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color buffer
	quadProgram_->use();
	quad_->bind();
	quad_->render();
	quad_->unbind();
}

#ifdef SST_PROFILING
static int profiling_line{ 0 };
#endif

void Window::renderStereoscopic(const float &deltaTime)
{
#ifdef SST_PROFILING
	auto t1 = std::chrono::high_resolution_clock::now();
#endif

	// First pass
	glEnable(GL_DEPTH_TEST);
	framebuffer_->bind(); // Render the scene on a framebuffer
	glViewport(0, 0, framebuffer_->getWidth(), framebuffer_->getHeight()); // Viewport for framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
	baseProgram_->use();
	camera_->update(deltaTime, *baseProgram_);
	light_->update(baseProgram_);
	for (WavefrontObject *obj : objs_) { obj->draw(*baseProgram_); }
	framebuffer_->unbind();
	// End First pass

#ifdef SST_PROFILING
	auto t2 = std::chrono::high_resolution_clock::now();
#endif

	// Second pass
	glDisable(GL_DEPTH_TEST);
	quad_->bind();
	glViewport(0, 0, frameSize_.width / 2, frameSize_.height);
	quadProgram_->use();
	framebuffer_->bindColorTexture(*quadProgram_); // Render color on the left
	quad_->render();

#ifdef SST_PROFILING
	auto t3 = std::chrono::high_resolution_clock::now();
#endif

	glViewport(frameSize_.width / 2, 0, frameSize_.width / 2, frameSize_.height);
	depthProgram_->use();
	framebuffer_->bindDepthTexture(*depthProgram_); // Render depth on the right
	quad_->render();
	quad_->unbind();
	// End Second pass

#ifdef SST_PROFILING
	auto t4 = std::chrono::high_resolution_clock::now();

	float pass1 = (t2 - t1).count(); // Pass 1
	float pass2_1 = (t3 - t2).count(); // Pass 2.1
	float pass2_2 = (t4 - t3).count(); // Pass 2.2
	float pass2 = pass2_1 + pass2_2;
	float total = pass1 + pass2_1 + pass2_2;
	std::cout << ++profiling_line << "\t" // Frame number
		<< pass1 << "\t" // Pass 1
		<< pass2_1 << "\t" // Pass 2.1
		<< pass2_2 << "\t" // Pass 2.2
		<< total << "\t" // Total
		<< pass2 / total * 100.0f // Overhead
		<< std::endl;
#endif
}
