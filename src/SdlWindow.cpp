#include <iostream>

#include "SdlWindow.h"
#include "Camera.h"
#include "Framebuffer.h"


const std::string SdlWindow::tag{ "SdlWindow" };


SdlWindow::SdlWindow(const unsigned width, const unsigned height, const std::string &title)
	: Window::Window{ width, height }
	, window_{ nullptr }
	, context_{ nullptr }
{
	// Initialize SDL video subsystem and handle error
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { throw SdlException(tag); }

	window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
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


void SdlWindow::render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model& model)
{
	Camera camera{ 45.0f, width_ / 2.0f / height_, 0.125f, 8.0f };

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	SDL_Event sdlEvent;
	while (true) {
		while (SDL_PollEvent(&sdlEvent)) { // Processes events that have already been received
			if (sdlEvent.type == SDL_QUIT) { goto EXIT; }
		}

		glViewport(0, 0, width_, height_);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer

		model.bind();

		glViewport(0, 0, width_ / 2, height_);  // Render color sub-image
		baseProgram.use();
		camera.update(baseProgram);
		model.rotateY(0.0025f);
		model.render(baseProgram);

		glViewport(width_ / 2, 0, width_ / 2, height_); // Render depth sub-image
		glDisable(GL_DEPTH_TEST);
		depthProgram.use();
		camera.update(depthProgram);
		model.render(depthProgram);

		model.unbind();

		SDL_GL_SwapWindow(window_); // Swap back buffer to the front
	}
EXIT:
	return;
}


void SdlWindow::render(const ShaderProgram &program, const Quad& quad)
{
	program.use();

	SDL_Event sdlEvent;
	while (true) {
		while (SDL_PollEvent(&sdlEvent)) { // Processes events that have already been received
			if (sdlEvent.type == SDL_QUIT) { goto EXIT; }
		}

		glViewport(0, 0, width_, height_);

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

		quad.bind();
		quad.render();
		quad.unbind();

		SDL_GL_SwapWindow(window_); // Swap back buffer to the front
	}
EXIT:
	return;
}


void SdlWindow::render(const ShaderProgram &modelProgram, const ShaderProgram &depthProgram, Model &model,
	const ShaderProgram &quadProgram, const Quad &quad)
{
	int width = width_;
	int height = height_;

	//SDL_GL_GetDrawableSize(window_, &width, &height);
	Framebuffer frame{ static_cast<unsigned>(width), static_cast<unsigned>(height / 2) };
	float aspectRatio{ (width / 2.0f) / (height / 2.0f) };
	Camera camera{ 45.0f, aspectRatio, 0.125f, 8.0f };

	double lastTime{ SDL_GetTicks() / 1000.0 };
	double currentTime{ SDL_GetTicks() / 1000.0 };
	float deltaTime{ static_cast<float>(currentTime - lastTime) };

	SDL_Event sdlEvent;
	while (true) {
		while (SDL_PollEvent(&sdlEvent)) { // Processes events that have already been received
			switch (sdlEvent.type) {
			case SDL_KEYUP:
				switch (sdlEvent.key.keysym.sym) {
				case SDLK_ESCAPE: goto EXIT;
				case SDLK_f: toggleFullscreen(); break;
				default: break;
				}
			default: break;
			}
			if (sdlEvent.type == SDL_QUIT) { goto EXIT; }
		}
		SDL_GL_GetDrawableSize(window_, &width, &height);

		currentTime = SDL_GetTicks() / 1000.0;
		deltaTime = static_cast<float>(currentTime - lastTime);
		lastTime = currentTime;

		frame.bind(); // First pass: render the scene on a framebuffer
		glViewport(0, 0, width, height / 2); // Viewport for color and depth sub-images
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer

		model.bind();
		model.rotateY(deltaTime);
		modelProgram.use();
		camera.update(modelProgram);
		glViewport(0, 0, width / 2, height / 2); // Render color sub-image
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

		SDL_GL_SwapWindow(window_); // Swap back buffer to the front
	}
EXIT:
	return;
}