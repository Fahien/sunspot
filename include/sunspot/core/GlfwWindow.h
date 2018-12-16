#pragma once

#include <logspot/Logger.h>

#include "sunspot/system/graphic/Shader.h"
#include "sunspot/system/graphic/Exception.h"
#include "Cursor.h"
#include "Window.h"


struct GLFWmonitor;
struct GLFWvidmode;

namespace sunspot
{


class GlfwException : public graphic::Exception
{
public:
	GlfwException(const std::string& tag, const std::string& message)
		: graphic::Exception{ tag + ": " + message } {}
};


class GlfwWindow : public Window
{
public:
	GlfwWindow( Game& game, const char* title = "SunSpot", const mst::Size windowSize = { 960, 540 }, const bool stereoscopic = false);
	~GlfwWindow();

	GLFWwindow* GetHandle() const { return m_Window; }

	bool ShouldClose() const { return glfwWindowShouldClose( m_Window ); }
	void PollEvents() const { glfwPollEvents(); }
	void SwapBuffers() const {
		glfwMakeContextCurrent( m_Window );
		glfwSwapBuffers( m_Window );
	}
	const float GetTime() const { return static_cast<float>( glfwGetTime() ); }
	void UpdateSize();

	void loop();

	struct Context
	{
		struct
		{
			int major;
			int minor;
		} version;
		int profile;
	} context;

protected:
	void toggleFullscreen();
	const input::Input pollInput();
	void render(const float& deltaTime);

private:
	void handleMouse(const int action);
	void handleMouse(const double x, const double y);
	void handleInput(const int key, const int action);

	void renderMesh(const float& deltaTime);
	void render3D(const float& deltaTime);
	void render3DplusDepth(const float& deltaTime);
	void renderQuad(const float& deltaTime);
	void renderStereoscopic(const float& deltaTime);

	GLFWmonitor*       m_Monitor;
	const GLFWvidmode* m_VideoMode;
	GLFWwindow*        m_Window;
};

}
