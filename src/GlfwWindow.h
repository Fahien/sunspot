#ifndef SST_GLFWWINDOW_H
#define SST_GLFWWINDOW_H

#include "Window.h"


class GlfwException : public GraphicException {
public:
	GlfwException(const std::string &tag, const std::string &message) : GraphicException{ tag + ": " + message } {}
};


class GlfwWindow : public Window
{
  public:
	static const std::string tag;
	GlfwWindow(const unsigned width, const unsigned height, const std::string &title);
	~GlfwWindow();

	void handleInput(int key);
	void toggleFullscreen();

	void render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model &model);
	void render(const ShaderProgram &program, const Quad &quad);
	void render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model &model,
		const ShaderProgram &quadProgram, const Quad &quad);

  private:
	bool rotateY_;
	GLFWwindow *window_;
	const GLFWvidmode* videoMode_;
};

#endif