#include <iostream>

#include "SdlWindow.h"
#include "Camera.h"

using namespace sunspot;


const std::string SdlWindow::tag{ "SdlWindow" };


SdlWindow::SdlWindow(const char *title, const int width, const int height, const bool decorated)
	: Window::Window{ title, width, height }
	, window_{ nullptr }
	, context_{ nullptr }
	, event_{}
{
	// Initialize SDL video subsystem and handle error
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { throw SdlException(tag); }

	Uint32 flags{ SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI };
	if (!decorated) { flags |= SDL_WINDOW_BORDERLESS; }
	window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (window_ == nullptr) { // Handle window creation error
		SDL_Quit();
		throw SdlException(tag);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // Request OpenGL 3.3 context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Turn on double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // Set 24 bit Z buffer
	context_ = SDL_GL_CreateContext(window_);
	if (context_ == nullptr) { // Handle context creation error
		SDL_DestroyWindow(window_);
		SDL_Quit();
		throw SdlException(tag);
	}
	SDL_GetWindowSize(window_, &monitorSize_.width, &monitorSize_.height); // Get monitor size
	SDL_SetRelativeMouseMode(SDL_TRUE); // Hide cursor, and report continuous motion

	try { Window::initGlew(); } // Initialize glew
	catch (const GlewException &) {
		SDL_GL_DeleteContext(context_);
		SDL_DestroyWindow(window_);
		SDL_Quit();
		throw;
	}

	updateFrameSize();
	SDL_GL_SetSwapInterval(1); // Syncronize with the monitor's vertical refresh

	SDL_version version;
	SDL_GetVersion(&version);
	std::cout << tag << ": created\n\tOpenGL " << glGetString(GL_VERSION) << std::endl;
	std::printf("\tSDL %d.%d.%d\n", version.major, version.minor, version.patch);
	std::cout << "\tFrame size " << frameSize_.width << "x" << frameSize_.height << std::endl;
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
	}
	else { // Set fullscreen mode
		if (SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0) { // Handle error
			throw SdlException(tag);
		}
	}
	fullscreen_ = !fullscreen_;
}


void SdlWindow::loop()
{
	while (true) {
		while (SDL_PollEvent(&event_)) { // Processes events that have already been received
			float sensitivity{ cursor_.getSensitivity() * deltaTime_ };
			switch (event_.type) {
			  case SDL_MOUSEMOTION: // Look around
				camera_->setYaw(camera_->getYaw() - event_.motion.xrel * sensitivity);
				camera_->setPitch(camera_->getPitch() + event_.motion.yrel * sensitivity);
				camera_->updateVectors();
				break;
			  case SDL_KEYUP:
				switch (event_.key.keysym.sym) {
				  case SDLK_w: camera_->setVelocityZ(0.0f); break;
				  case SDLK_s: camera_->setVelocityZ(0.0f); break;
				  case SDLK_a: camera_->setVelocityX(0.0f); break;
				  case SDLK_d: camera_->setVelocityX(0.0f); break;
				  case SDLK_f: toggleFullscreen(); break;
				  case SDLK_q:
				  case SDLK_ESCAPE: goto EXIT;
				  default: break;
				}
				break;
			  case SDL_KEYDOWN:
				switch (event_.key.keysym.sym) {
					case SDLK_w: camera_->setVelocityZ(-1.0f); break;
					case SDLK_s: camera_->setVelocityZ(1.0f); break;
					case SDLK_a: camera_->setVelocityX(-1.0f); break;
					case SDLK_d: camera_->setVelocityX(1.0f); break;
					default: break;
				}
				break;
			  case SDL_QUIT: goto EXIT;
			  default: break;
			}
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


void SdlWindow::updateFrameSize()
{
	// Get the size of a window's underlying drawable in pixels
	SDL_GL_GetDrawableSize(window_, &frameSize_.width, &frameSize_.height);
}

