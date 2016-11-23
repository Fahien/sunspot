#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include <string>
#include "Math.h"

class ShaderProgram;
class Light;
class Model;
class Quad;
class Camera;
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

	Window(const unsigned width, const unsigned height, const char *title);

	inline void setBaseProgram(const ShaderProgram *baseProgram) { baseProgram_ = baseProgram; }
	inline void setDepthProgram(const ShaderProgram *depthProgram) { depthProgram_ = depthProgram; }
	inline void setLight(Light *light) { light_ = light; }
	inline void setModel(Model *model) { model_ = model; }
	inline void setRoom(Model *room) { room_ = room; }

	inline void setQuadProgram(const ShaderProgram *quadProgram) { quadProgram_ = quadProgram; }
	inline void setQuad(Quad *quad) { quad_ = quad; }

	inline void setCamera(Camera *camera) { camera_ = camera; }
	inline void setFramebuffer(const Framebuffer *framebuffer) { framebuffer_ = framebuffer; }

	virtual void loop() = 0;

protected:
	static void initGlew();

	virtual void render();
	virtual const float &computeDeltaTime() = 0;
	virtual void render(const float &deltaTime) = 0;

	unsigned width_;
	unsigned height_;
	const char* title_;
	math::Size monitorSize_;
	math::Size frameSize_;
	

	float currentTime_;
	float lastTime_;
	float deltaTime_;

	const ShaderProgram *baseProgram_;
	const ShaderProgram *depthProgram_;
	Light *light_;
	Model *model_;
	Model *room_;

	const ShaderProgram *quadProgram_;
	const Quad *quad_;

	Camera *camera_;

	const Framebuffer *framebuffer_;

	bool fullscreen_;
};

#endif // SST_WINDOW_H
