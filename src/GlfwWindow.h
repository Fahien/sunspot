#ifndef SST_GLFWWINDOW_H
#define SST_GLFWWINDOW_H

#include "Window.h"
#include "Cursor.h"


namespace sunspot {

class GlfwException : public GraphicException {
public:
	GlfwException(const std::string &tag, const std::string &message) : GraphicException{ tag + ": " + message } {}
};


class GlfwWindow : public Window
{
  public:
	static const std::string tag;
	GlfwWindow(const char *title, const int width, const int height, const bool stereo, const bool decorated);
	~GlfwWindow();

	void handleMouse(const double x, const double y);
	void handleInput(const int key, const int action);
	void toggleFullscreen();

	void loop();

  protected:
	const float &computeDeltaTime();
	void render(const float &deltaTime);

  private:
	void renderMesh(const float &deltaTime);
	void render3D(const float &deltaTime);
	void render3DplusDepth(const float &deltaTime);
	void renderQuad(const float &deltaTime);
	void renderStereoscopic(const float &deltaTime);

	bool rotateY_;
	GLFWmonitor *monitor_;
	const GLFWvidmode* videoMode_;
	GLFWwindow *window_;
	Cursor cursor_;
};

}

#endif // SST_GLFWWINDOW_H

