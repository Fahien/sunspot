#ifndef SST_GLFWWINDOW_H
#define SST_GLFWWINDOW_H

#include "Window.h"
#include "Cursor.h"


class GlfwException : public GraphicException {
public:
	GlfwException(const std::string &tag, const std::string &message) : GraphicException{ tag + ": " + message } {}
};


class GlfwWindow : public Window
{
  public:
	static const std::string tag;
	GlfwWindow(const unsigned width, const unsigned height, const char *title);
	~GlfwWindow();

	void handleMouse(const double x, const double y);
	void handleInput(const int key, const int action);
	void toggleFullscreen();

	void loop();

  protected:
	const float &computeDeltaTime();
	void render(const float &deltaTime) const;

  private:
	void render3D(const float &deltaTime) const;
	void render3DplusDepth(const float &deltaTime) const;
	void renderQuad(const float &deltaTime) const;
	void renderStereoscopic(const float &deltaTime) const;

	bool rotateY_;
	GLFWwindow *window_;
	const GLFWvidmode* videoMode_;
	Cursor cursor_;
};

#endif