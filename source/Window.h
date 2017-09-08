#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include <string>
#include <stdexcept>
#include <vector>
#include <MathSpot.h>

#include "Graphics.h"
#include "Cursor.h"

namespace mst = mathspot;


namespace sunspot {

class Quad;
class ShaderProgram;
class Light;
class WavefrontObject;
class Framebuffer;
class Camera;


class GlewException : public GraphicException {
public:
	GlewException(const std::string& tag) : GraphicException{ tag + ": Could not initialize GLEW" } {}
};


class Window {
public:
	static const std::string tag;

	Window(const char *title, const mst::Size windowSize, const bool decorated, const bool stereoscopic);

	inline mst::Size &getFrameSize() { return frameSize_; }
	inline void setBaseProgram(const ShaderProgram *baseProgram) { baseProgram_ = baseProgram; }
	inline void setLight(Light *light) { light_ = light; }
	inline void addObj(WavefrontObject *obj) { objs_.push_back(obj); }

	inline void setQuad(Quad *quad) { quad_ = quad; }
	inline void setQuadProgram(const ShaderProgram *quadProgram) { quadProgram_ = quadProgram; }
	inline void setDepthProgram(const ShaderProgram *depthProgram) { depthProgram_ = depthProgram; }


	inline void setCamera(Camera *camera) { camera_ = camera; }
	inline void setFramebuffer(const Framebuffer *framebuffer) { framebuffer_ = framebuffer; }

	virtual void loop() = 0;

protected:
	static void initGlew();
	void render();

	virtual void toggleFullscreen() = 0;
	virtual const float &computeDeltaTime() = 0;
	virtual void updateFrameSize() = 0;

	const char* title_;
	mst::Size windowSize_;
	mst::Size monitorSize_;
	mst::Size frameSize_;

	bool decorated_;
	bool stereoscopic_;
	bool fullscreen_;

	float currentTime_;
	float lastTime_;
	float deltaTime_;

	Cursor cursor_;
	Camera* camera_;

private:
	void render(const float &deltaTime);
	void render3D(const float &deltaTime);
	void renderQuad(const float &deltaTime);
	void renderStereoscopic(const float &deltaTime);

	const ShaderProgram *baseProgram_;
	Light *light_;
	std::vector<WavefrontObject *> objs_;
	const ShaderProgram *quadProgram_;
	const ShaderProgram *depthProgram_;
	const Framebuffer *framebuffer_;
	const Quad *quad_;
};


}


#endif // SST_WINDOW_H
