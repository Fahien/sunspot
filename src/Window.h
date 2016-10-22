#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include<string>

#include "ShaderProgram.h"
#include "Model.h"
#include "Quad.h"


struct GLFWwindow;

class Window
{
  public:
	Window(const unsigned width, const unsigned height, const std::string &title);
	~Window();

	void render(const ShaderProgram &program, const Model &model);
	void render(const ShaderProgram &program, const Quad &quad);
	void render(const ShaderProgram &modelProgram, const Model &model, const ShaderProgram &program, const Quad &quad);
  private:
	GLFWwindow *window_;
};

#endif // SST_WINDOW_H
