#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include <string>
#include <stdexcept>
#include <vector>
#include <MathSpot.h>

#include <Graphics.h>
#include <Cursor.h>

namespace mst = mathspot;


namespace sunspot {

class Quad;
class ShaderProgram;
class Light;
class WavefrontObject;
class Framebuffer;
class Camera;


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

	inline mst::Size& getFrameSize() { return mFrameSize; }
	inline void setBaseProgram(const ShaderProgram* baseProgram) { mBaseProgram = baseProgram; }
	inline void setLight(Light* light) { mLight = light; }
	inline void addObj(WavefrontObject* obj) { mObjs.push_back(obj); }

	inline void setQuad(Quad* quad) { mQuad = quad; }
	inline void setQuadProgram(const ShaderProgram* quadProgram) { mQuadProgram = quadProgram; }
	inline void setDepthProgram(const ShaderProgram* depthProgram) { mDepthProgram = depthProgram; }


	inline void setCamera(Camera* camera) { mCamera = camera; }
	inline void setFramebuffer(const Framebuffer* framebuffer) { mFramebuffer = framebuffer; }

	virtual void loop() = 0;

protected:
	static void initGlew();
	void render();

	virtual void toggleFullscreen() = 0;
	virtual const float& computeDeltaTime() = 0;
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

	Cursor  mCursor;
	Camera* mCamera;

private:
	void render(const float &deltaTime);
	void render3D(const float& deltaTime);
	void renderQuad(const float& deltaTime);
	void renderStereoscopic(const float& deltaTime);

	const ShaderProgram* mBaseProgram;
	Light* mLight;
	std::vector<WavefrontObject*> mObjs;
	const ShaderProgram* mQuadProgram;
	const ShaderProgram* mDepthProgram;
	const Framebuffer* mFramebuffer;
	const Quad* mQuad;
};


}


#endif // SST_WINDOW_H
