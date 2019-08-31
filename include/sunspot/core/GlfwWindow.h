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
	GlfwWindow( Game& game, const std::string& title = "SunSpot", const mst::Size& window_size = { 960, 540 } );
	~GlfwWindow() override;

	GLFWwindow* get_handle() const { return handle; }

	void toggle_maximization()
	{
		maximized ? glfwRestoreWindow( handle ) : glfwMaximizeWindow( handle );
		maximized = !maximized;
	}

	void restore()
	{
		glfwRestoreWindow( handle );
		maximized = false;
	}

	void set_closing( const bool closing ) const;

	bool is_closing() const { return glfwWindowShouldClose( handle ); }

	void poll_events() const { glfwPollEvents(); }

	void swap_buffers() const
	{
		glfwMakeContextCurrent( handle );
		glfwSwapBuffers( handle );
	}

	const Win::Position& get_position()
	{
		glfwGetWindowPos( handle, &window.pos.x, &window.pos.y );
		return window.pos;
	}

	void set_position( const int x, const int y )
	{
		if ( !maximized )
		{
			window.pos = { x, y };
			glfwSetWindowPos( handle, x, y );
		}
	}

	const float get_time() const { return static_cast<float>( glfwGetTime() ); }

	void update_size();

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
	void toggle_fullscreen() override;

	const input::Input poll_input() override;

  private:
	void handle_mouse( const int action );
	void handle_mouse( const double x, const double y );
	void handle_input( const int key, const int action );

	GLFWmonitor*       monitor   = nullptr;
	const GLFWvidmode* videomode = nullptr;
	GLFWwindow*        handle    = nullptr;
	bool               maximized = false;
};

}  // namespace sunspot
