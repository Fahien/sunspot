#ifndef SST_SDLWINDOW_H
#define SST_SDLWINDOW_H

#include <exception>

#include "Graphics.h"
#include "Window.h"


class SdlException : public GraphicException {
  public:
	SdlException(const std::string &tag) : GraphicException{ tag + ": " + SDL_GetError() } {}
};


class SdlWindow : public Window {
  public:
	static const std::string tag;
	SdlWindow(const unsigned width, const unsigned height, const char *title);
	~SdlWindow();

	void toggleFullscreen();

	void loop();

  protected:
	const float &computeDeltaTime();
	void render(const float &deltaTime) const;

  private:
	void render3DplusDepth(const float &deltaTime) const;
	void renderQuad(const float &deltaTime) const;
	void renderStereoscopic(const float &deltaTime) const;

	SDL_Window *window_;
	SDL_GLContext context_;
};

#endif // SST_SDLWINDOW_H