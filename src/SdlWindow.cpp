#include <iostream>

#include "SdlWindow.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Quad.h"
#include "Camera.h"
#include "Framebuffer.h"


const std::string SdlWindow::tag{ "SdlWindow" };


SdlWindow::SdlWindow(const unsigned width, const unsigned height, const char *title)
	: Window::Window{ width, height, title }
	, window_{ nullptr }
	, context_{ nullptr }
{
	// Initialize SDL video subsystem and handle error
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { throw SdlException(tag); }

	window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	if (window_ == nullptr) { // Handle window creation error
		SDL_Quit();
		throw SdlException(tag);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // Request OpenGL 3.3 context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Turn on double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Set 16 bit Z buffer
	context_ = SDL_GL_CreateContext(window_);
	if (context_ == nullptr) { // Handle context creation error
		SDL_DestroyWindow(window_);
		SDL_Quit();
		throw SdlException(tag);
	}

	int mWidth, mHeight; // Get monitor size
	SDL_GetWindowSize(window_, &mWidth, &mHeight);
	monitorWidth_ = static_cast<unsigned>(mWidth);
	monitorHeight_ = static_cast<unsigned>(mHeight);

	SDL_GL_SetSwapInterval(1); // Syncronize with the monitor's vertical refresh

	try {
		Window::initGlew();
	}
	catch (GlewException &) {
		SDL_GL_DeleteContext(context_);
		SDL_DestroyWindow(window_);
		SDL_Quit();
		throw;
	}

	std::cout << tag << ": created\n"; // TODO remove debug log
}


SdlWindow::~SdlWindow()
{
	if (context_ != nullptr) { SDL_GL_DeleteContext(context_); }
	if (window_ != nullptr) { SDL_DestroyWindow(window_); }
	SDL_Quit();
	std::cout << tag << ": destroyed\n"; // TODO remove debug log
}


void SdlWindow::toggleFullscreen()
{
	if (fullscreen_) {  // Set windowed mode
		if (SDL_SetWindowFullscreen(window_, 0) < 0) { // Handle error
			throw SdlException(tag);
		}
		fullscreen_ = false;
	}
	else { // Set fullscreen mode
		if (SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0) { // Handle error
			throw SdlException(tag);
		}
		fullscreen_ = true;
	}
}


void SdlWindow::loop()
{
	SDL_Event sdlEvent;
	while (true) {
		while (SDL_PollEvent(&sdlEvent)) { // Processes events that have already been received
			switch (sdlEvent.type) {
			case SDL_KEYUP:
				switch (sdlEvent.key.keysym.sym) {
				case SDLK_q:
				case SDLK_ESCAPE: goto EXIT;
				case SDLK_f: toggleFullscreen(); break;
				default: break;
				}
			default: break;
			}
			if (sdlEvent.type == SDL_QUIT) { goto EXIT; }
		}
		Window::render();
		SDL_GL_SwapWindow(window_); // Swap back buffer to the front
	}
EXIT:
	return;
}


const float &SdlWindow::computeDeltaTime()
{
	currentTime_ = static_cast<float>(SDL_GetTicks() / 1000.0);
	deltaTime_ = currentTime_ - lastTime_;
	lastTime_ = currentTime_;
	return deltaTime_;
}


void SdlWindow::render(const float &deltaTime) const
{
	render3DplusDepth(deltaTime);
}


void SdlWindow::render3DplusDepth(const float &deltaTime) const
{
	glViewport(0, 0, width_, height_);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer

	model_->bind();

	glViewport(0, 0, width_ / 2, height_);  // Render color sub-image
	baseProgram_->use();
	camera_->update(deltaTime, baseProgram_);
	model_->transform.rotateY(0.0025f);
	model_->render(baseProgram_);

	glViewport(width_ / 2, 0, width_ / 2, height_); // Render depth sub-image
	glDisable(GL_DEPTH_TEST);
	depthProgram_->use();
	camera_->update(deltaTime, depthProgram_);
	model_->render(depthProgram_);

	model_->unbind();
}


void SdlWindow::renderQuad(const float &deltaTime) const
{
	depthProgram_->use();
	glViewport(0, 0, width_, height_);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

	quad_->bind();
	quad_->render();
	quad_->unbind();
}


void SdlWindow::renderStereoscopic(const float &deltaTime) const
{
	int width, height;
	SDL_GL_GetDrawableSize(window_, &width, &height);

	framebuffer_->bind(); // First pass: render the scene on a framebuffer
	glViewport(0, 0, width, height / 2); // Viewport for color and depth sub-images
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer

	model_->bind();
	model_->transform.rotateY(deltaTime);
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