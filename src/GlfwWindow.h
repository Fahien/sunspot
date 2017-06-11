#ifndef SST_GLFWWINDOW_H
#define SST_GLFWWINDOW_H

#include "Window.h"
#include "Cursor.h"
#include "Logger.h"


struct GLFWmonitor;
struct GLFWvidmode;

namespace sunspot {

class GlfwException : public GraphicException {
public:
	GlfwException(const std::string &tag, const std::string &message)
		: GraphicException{ tag + ": " + message } {}
};


class GlfwWindow : public Window
{
public:
	static const Logger log;
	static const std::string tag;
	GlfwWindow(const char *title, const math::Size windowSize, const bool decorated, const bool stereoscopic);
	~GlfwWindow();

	void loop();

protected:
	void toggleFullscreen();
	const float &computeDeltaTime();
	void updateFrameSize();
	void render(const float &deltaTime);

private:
	void handleMouse(const double x, const double y);
	void handleInput(const int key, const int action);

	void renderMesh(const float &deltaTime);
	void render3D(const float &deltaTime);
	void render3DplusDepth(const float &deltaTime);
	void renderQuad(const float &deltaTime);
	void renderStereoscopic(const float &deltaTime);

	GLFWmonitor       *monitor_;
	const GLFWvidmode *videoMode_;
	GLFWwindow        *window_;
};

}

#endif // SST_GLFWWINDOW_H
