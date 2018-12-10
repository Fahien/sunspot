#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include <string>
#include <stdexcept>
#include <vector>
#include <memory>
#include <optional>

#include <mathspot/Math.h>

#include "sunspot/system/Graphic.h"
#include "sunspot/system/graphic/Framebuffer.h"
#include "Cursor.h"
#include "sunspot/input/Input.h"
#include "system/Collision.h"
#include "sunspot/component/Camera.h"

namespace mst = mathspot;


namespace sunspot {

class Quad;
class Light;
class WavefrontObject;
class Camera;
class Entity;

class GltfRenderer;
class GltfCamera;


class GlewException : public graphic::Exception
{
public:
	GlewException(const std::string& tag) : graphic::Exception{ tag + ": Could not initialize GLEW" } {}
};


class Window
{
public:
	static const std::string tag;

	Window(const char* title, const mst::Size windowSize, const bool decorated, const bool stereoscopic);
	~Window();

	const mst::Size& getFrameSize() const { return m_FrameSize; }
	inline void setBaseProgram(const graphic::shader::Program* baseProgram) { mBaseProgram = baseProgram; }
	inline void setLight(graphic::Light* light) { mLight = light; }
	inline void addObj(WavefrontObject* obj) { mObjs.push_back(obj); }
	inline void AddGltf(GltfRenderer* renderer) { mGltfRenderer = renderer; }
	inline void AddEntity(Entity* entity) { mEntities.push_back(entity); mCollision.Add(*entity); }

	inline void setQuad(Quad* quad) { mQuad = quad; }
	inline void setQuadProgram(const graphic::shader::Program* quadProgram) { mQuadProgram = quadProgram; }
	inline void setDepthProgram(const graphic::shader::Program* depthProgram) { mDepthProgram = depthProgram; }


	void SetCamera( GltfCamera* camera ) { m_pCamera = camera; }
	void SetCamera( Entity& camera ) { m_Camera = &camera; }
	void setFramebuffer(const graphic::Framebuffer* framebuffer) { mFramebuffer = framebuffer; }

	virtual void loop() = 0;
	virtual void UpdateSize() = 0;

protected:
	static void initGlew();
	void render();
	void handleInput(input::Input in);

	virtual void toggleFullscreen() = 0;
	virtual const input::Input pollInput() = 0;

	const char* m_Title;
	mst::Size m_WindowSize;
	mst::Size m_MonitorSize;
	mst::Size m_FrameSize;

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

	const graphic::shader::Program* mBaseProgram;
	graphic::Light* mLight;
	std::vector<WavefrontObject*> mObjs;
	//std::vector<std::shared_ptr<Entity>> mEntities;
	std::vector<Entity*> mEntities{};
	GltfRenderer* mGltfRenderer { nullptr };
	const graphic::shader::Program* mQuadProgram;
	const graphic::shader::Program* mDepthProgram;
	const graphic::Framebuffer* mFramebuffer;
	const Quad* mQuad;

	system::Collision mCollision;
};


}


#endif // SST_WINDOW_H
