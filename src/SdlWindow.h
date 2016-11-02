#ifndef SST_SDLWINDOW_H
#define SST_SDLWINDOW_H

#include "Window.h"

class SdlWindow : public Window
{
  public:
	SdlWindow(const unsigned width, const unsigned height, const std::string &title);
	~SdlWindow();

	void render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model& model);
	void render(const ShaderProgram &program, const Quad &quad) {}
	void render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model &model,
		const ShaderProgram &quadProgram, const Quad &quad) {}

  private:
	SDL_Window *window_;
	SDL_GLContext context_;
};

#endif // SST_SDLWINDOW_H