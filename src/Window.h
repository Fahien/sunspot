#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include <string>
#include <stdexcept>
#include "Math.h"
#include "Cursor.h"

class ShaderProgram;
class Model;
class Quad;
class Camera;


namespace sunspot {

class Light;
class Mesh;
class Framebuffer;

class GraphicException : public std::runtime_error {
public:
	GraphicException(const std::string& message) : std::runtime_error(message) {}
};


class GlewException : public GraphicException {
public:
	GlewException(const std::string& tag) : GraphicException{ tag + ": Could not initialize GLEW" } {}
};


class Window {
public:
	static const std::string tag;

	Window(const char *title, const math::Size windowSize, const bool decorated, const bool stereoscopic);

	inline math::Size &getFrameSize() { return frameSize_; }
	inline void setBaseProgram(const ShaderProgram *baseProgram) { baseProgram_ = baseProgram; }
	inline void setLight(Light *light) { light_ = light; }
	inline void setRoom(Model *room) { room_ = room; }
	inline void setMesh(Mesh *mesh) { mesh_ = mesh; }

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
	math::Size windowSize_;
	math::Size monitorSize_;
	math::Size frameSize_;

	bool decorated_;
	bool stereoscopic_;
	bool fullscreen_;

	float currentTime_;
	float lastTime_;
	float deltaTime_;

	Cursor cursor_;
	Camera *camera_;

private:
	void render(const float &deltaTime);
	void render3D(const float &deltaTime);
	void renderQuad(const float &deltaTime);
	void renderStereoscopic(const float &deltaTime);

	const ShaderProgram *baseProgram_;
	Light *light_;
	Model *room_;
	Mesh *mesh_;
	const ShaderProgram *quadProgram_;
	const ShaderProgram *depthProgram_;
	const Framebuffer *framebuffer_;
	const Quad *quad_;
};

}

#endif // SST_WINDOW_H

