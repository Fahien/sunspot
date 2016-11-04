#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include<string>

#include "ShaderProgram.h"
#include "Model.h"
#include "Quad.h"


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
	Window(const unsigned width, const unsigned height);

	// TODO refactor rendering functions and add virtual input handling functions
	virtual void render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model &model) = 0;
	virtual void render(const ShaderProgram &program, const Quad &quad) = 0;
	virtual void render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model &model,
		                const ShaderProgram &quadProgram, const Quad &quad) = 0;

  protected:
	static void initGlew();

	unsigned width_;
	unsigned height_;

	bool fullscreen_;
};

#endif // SST_WINDOW_H