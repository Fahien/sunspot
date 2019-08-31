#pragma once

#include <logspot/Log.h>

#include "Cursor.h"
#include "Window.h"
#include "sunspot/graphics/Exception.h"
#include "sunspot/graphics/Shader.h"


struct GLFWmonitor;
struct GLFWvidmode;

namespace sunspot
{
class GlfwException : public graphics::Exception
{
  public:
	GlfwException( const std::string& tag, const std::string& message )
	    : graphics::Exception{ tag + ": " + message }
	{
	}
};


class GlfwWindow : public Window
{
  public:
	GlfwWindow( Game& game, const std::string& title = "SunSpot", const mst::Size& window_size = { 960, 540 },
	            const bool stereoscopic = false );
	~GlfwWindow();

	GLFWwindow* GetHandle() const { return handle; }

	void ToggleMaximization()
	{
		maximized ? glfwRestoreWindow( handle ) : glfwMaximizeWindow( handle );
		maximized = !maximized;
	}
	void Restore()
	{
		glfwRestoreWindow( handle );
		maximized = false;
	}
	void SetClosing( const bool closing ) const;
	bool IsClosing() const { return glfwWindowShouldClose( handle ); }
	void PollEvents() const { glfwPollEvents(); }
	void SwapBuffers() const
	{
		glfwMakeContextCurrent( handle );
		glfwSwapBuffers( handle );
	}
	const Win::Position& GetPosition()
	{
		glfwGetWindowPos( handle, &window.pos.x, &window.pos.y );
		return window.pos;
	}
	void SetPosition( const int x, const int y )
	{
		if ( !maximized )
		{
			window.pos = { x, y };
			glfwSetWindowPos( handle, x, y );
		}
	}
	const float GetTime() const { return static_cast<float>( glfwGetTime() ); }
	void        UpdateSize();

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
	void               toggleFullscreen();
	const input::Input pollInput();

  private:
	void handleMouse( const int action );
	void handleMouse( const double x, const double y );
	void handleInput( const int key, const int action );

	GLFWmonitor*       monitor;
	const GLFWvidmode* videomode;
	GLFWwindow*        handle;
	bool               maximized = false;
};

}  // namespace sunspot
