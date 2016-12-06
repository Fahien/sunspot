#ifndef SST_SDLWINDOW_H
#define SST_SDLWINDOW_H

#include <exception>

#include "Graphics.h"
#include "Window.h"


namespace sunspot {

class SdlException : public GraphicException {
  public:
	SdlException(const std::string &tag) : GraphicException{ tag + ": " + SDL_GetError() } {}
};


class SdlWindow : public Window {
  public:
	static const std::string tag;
	SdlWindow(const char *title, const int width, const int height, const bool decorated);
	~SdlWindow();

	void loop();

  protected:
	void toggleFullscreen();
	const float &computeDeltaTime();
	void updateFrameSize();
	void render(const float &deltaTime);

  private:
	SDL_Window *window_;
	SDL_GLContext context_;
	SDL_Event event_;
};

}

#endif // SST_SDLWINDOW_H

