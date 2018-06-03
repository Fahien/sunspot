#ifndef SST_GLFWWINDOW_H_
#define SST_GLFWWINDOW_H_

#include <Logger.h>

#include "Cursor.h"
#include "Window.h"


struct GLFWmonitor;
struct GLFWvidmode;

namespace sunspot
{


class GlfwException : public GraphicException
{
public:
	GlfwException(const std::string& tag, const std::string& message)
		: GraphicException{ tag + ": " + message } {}
};


class GlfwWindow : public Window
{
public:
	GlfwWindow(const char* title, const mst::Size windowSize, const bool decorated, const bool stereoscopic);
	~GlfwWindow();

	void loop();

protected:
	void toggleFullscreen();
	const float& computeDeltaTime();
	const input::Key& pollInput();
	void updateFrameSize();
	void render(const float& deltaTime);

private:
	void handleMouse(const double x, const double y);
	void handleInput(const int key, const int action);

	void renderMesh(const float& deltaTime);
	void render3D(const float& deltaTime);
	void render3DplusDepth(const float& deltaTime);
	void renderQuad(const float& deltaTime);
	void renderStereoscopic(const float& deltaTime);

	GLFWmonitor*       mMonitor;
	const GLFWvidmode* mVideoMode;
	GLFWwindow*        mWindow;
};

}

#endif // SST_GLFWWINDOW_H_
