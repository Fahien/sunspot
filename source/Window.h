#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <mathspot/Math.h>

#include "Cursor.h"
#include "sunspot/core/Collisions.h"
#include "sunspot/graphics/Graphics.h"
#include "sunspot/input/Input.h"

namespace mst = mathspot;


namespace sunspot
{
class Game;
class Quad;
class Light;
class Camera;
class Entity;


class GlewException : public graphics::Exception
{
  public:
	GlewException( const std::string& tag )
	    : graphics::Exception{ tag + ": Could not initialize GLEW" }
	{
	}
};


class Window
{
  public:
	Window( Game& game, const std::string& title, const mst::Size& window_size );
	virtual ~Window() = default;

	const mst::Size& get_frame_size() const { return frame_size; }

  protected:
	static void init_glew();
	void        handle( input::Input&& in );
	Game&       get_game() { return game; }

	virtual void toggle_fullscreen() = 0;

	virtual const input::Input poll_input() = 0;

	std::string title;

	// TODO refactor this
	struct Win
	{
		// TODO Create template size
		mst::Size size;
		// TODO create template vectors
		struct Position
		{
			int x;
			int y;
		} pos;
	} window;
	mst::Size monitor_size;
	mst::Size frame_size;

	bool fullscreen;

	input::Type   type     = input::Type::INVALID;
	input::Key    key      = input::Key::NONE;
	input::Action action   = input::Action::RELEASE;
	mst::Vec2     position = { 0.0f, 0.0f };

	Cursor cursor;

  private:
	Game& game;
};


}  // namespace sunspot


#endif  // SST_WINDOW_H
