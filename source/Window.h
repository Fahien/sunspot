#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include <string>
#include <stdexcept>
#include <vector>
#include <memory>
#include <optional>

#include <mathspot/Math.h>
#include "Graphics.h"
#include "Cursor.h"
#include "sunspot/input/Input.h"
#include "system/Collision.h"
#include "sunspot/component/Camera.h"

namespace mst = mathspot;


namespace sunspot {

class Quad;
class ShaderProgram;
class Light;
class WavefrontObject;
class Framebuffer;
class Camera;
class Entity;

class GltfRenderer;
class GltfCamera;


class GlewException : public GraphicException
{
public:
	GlewException(const std::string& tag) : GraphicException{ tag + ": Could not initialize GLEW" } {}
};


class Window
{
public:
	static const std::string tag;

	Window(const char* title, const mst::Size windowSize, const bool decorated, const bool stereoscopic);
	~Window();

	inline mst::Size& getFrameSize() { return mFrameSize; }
	inline void setBaseProgram(const ShaderProgram* baseProgram) { mBaseProgram = baseProgram; }
	inline void setLight(Light* light) { mLight = light; }
	inline void addObj(WavefrontObject* obj) { mObjs.push_back(obj); }
	inline void AddGltf(GltfRenderer* renderer) { mGltfRenderer = renderer; }
	inline void AddEntity(Entity* entity) { mEntities.push_back(entity); mCollision.Add(*entity); }

	inline void setQuad(Quad* quad) { mQuad = quad; }
	inline void setQuadProgram(const ShaderProgram* quadProgram) { mQuadProgram = quadProgram; }
	inline void setDepthProgram(const ShaderProgram* depthProgram) { mDepthProgram = depthProgram; }


	void SetCamera( GltfCamera* camera ) { m_pCamera = camera; }
	void SetCamera( Entity& camera ) { m_Camera = &camera; }
	void setFramebuffer(const Framebuffer* framebuffer) { mFramebuffer = framebuffer; }

	virtual void loop() = 0;

protected:
	static void initGlew();
	void render();
	void handleInput(input::Input in);

	virtual void toggleFullscreen() = 0;
	virtual const float& computeDeltaTime() = 0;
	virtual const input::Input pollInput() = 0;
	virtual void updateFrameSize() = 0;

	const char* mTitle;
	mst::Size mWindowSize;
	mst::Size mMonitorSize;
	mst::Size mFrameSize;

	bool mDecorated;
	bool mStereoscopic;
	bool mFullscreen;

	float mCurrentTime;
	float mLastTime;
	float mDeltaTime;

	input::Type   mType  { input::Type::INVALID   };
	input::Key    mKey   { input::Key::NONE       };
	input::Action mAction{ input::Action::RELEASE };
	mst::Vec2 mPosition{ 0.0f, 0.0f };

	Cursor  mCursor;

	GltfCamera* m_pCamera;
	Entity* m_Camera;

private:
	void render(const float &deltaTime);
	void render3D(const float& deltaTime);
	void renderGltf(const float& deltaTime);
	void renderQuad(const float& deltaTime);
	void renderStereoscopic(const float& deltaTime);

	const ShaderProgram* mBaseProgram;
	Light* mLight;
	std::vector<WavefrontObject*> mObjs;
	//std::vector<std::shared_ptr<Entity>> mEntities;
	std::vector<Entity*> mEntities{};
	GltfRenderer* mGltfRenderer { nullptr };
	const ShaderProgram* mQuadProgram;
	const ShaderProgram* mDepthProgram;
	const Framebuffer* mFramebuffer;
	const Quad* mQuad;

	system::Collision mCollision;
};


}


#endif // SST_WINDOW_H
