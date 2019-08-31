#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <mathspot/Math.h>

#include "Cursor.h"
#include "sunspot/component/Camera.h"
#include "sunspot/core/Collisions.h"
#include "sunspot/graphics/Graphics.h"
#include "sunspot/input/Input.h"

namespace mst = mathspot;


namespace sunspot
{
class Game;
class Quad;
class Light;
class WavefrontObject;
class Camera;
class Entity;

class GltfRenderer;
class GltfCamera;


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
	static const std::string tag;

	Window( Game& game, const std::string& title, const mst::Size& window_size, const bool stereoscopic );
	~Window();

	const mst::Size& getFrameSize() const { return m_FrameSize; }
	inline void      setBaseProgram( const graphics::shader::Program* baseProgram ) { mBaseProgram = baseProgram; }
	inline void      setLight( graphics::Light* light ) { mLight = light; }
	inline void      addObj( WavefrontObject* obj ) { mObjs.push_back( obj ); }
	inline void      AddGltf( GltfRenderer* renderer ) { mGltfRenderer = renderer; }

	inline void setQuad( Quad* quad ) { mQuad = quad; }
	inline void setQuadProgram( const graphics::shader::Program* quadProgram ) { mQuadProgram = quadProgram; }
	inline void setDepthProgram( const graphics::shader::Program* depthProgram ) { mDepthProgram = depthProgram; }


	void SetCamera( GltfCamera* camera ) { m_pCamera = camera; }
	void SetCamera( Entity& camera ) { m_Camera = &camera; }

	virtual void UpdateSize() = 0;

  protected:
	static void initGlew();
	void        handleInput( input::Input&& in );
	Game&       get_game() { return game; }

	virtual void               toggleFullscreen() = 0;
	virtual const input::Input pollInput()        = 0;

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
	mst::Size m_MonitorSize;
	mst::Size m_FrameSize;

	bool mStereoscopic;
	bool mFullscreen;

	float mCurrentTime;
	float mLastTime;
	float mDeltaTime;

	input::Type   mType{ input::Type::INVALID };
	input::Key    mKey{ input::Key::NONE };
	input::Action mAction{ input::Action::RELEASE };
	mst::Vec2     mPosition{ 0.0f, 0.0f };

	Cursor mCursor;

	GltfCamera* m_pCamera;
	Entity*     m_Camera;

  private:
	Game& game;

	const graphics::shader::Program* mBaseProgram;
	graphics::Light*                 mLight;
	std::vector<WavefrontObject*>    mObjs;
	GltfRenderer*                    mGltfRenderer{ nullptr };
	const graphics::shader::Program* mQuadProgram;
	const graphics::shader::Program* mDepthProgram;
	const Quad*                      mQuad;
};


}  // namespace sunspot


#endif  // SST_WINDOW_H
