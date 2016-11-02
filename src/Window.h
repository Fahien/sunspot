#ifndef SST_WINDOW_H
#define SST_WINDOW_H

#include<string>

#include "ShaderProgram.h"
#include "Model.h"
#include "Quad.h"

class Window
{
  public:
	Window(const unsigned width, const unsigned height);

	virtual void render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model &model) = 0;
	virtual void render(const ShaderProgram &program, const Quad &quad) = 0;
	virtual void render(const ShaderProgram &baseProgram, const ShaderProgram &depthProgram, Model &model,
		                const ShaderProgram &quadProgram, const Quad &quad) = 0;

  protected:
	static void initGlew();

	unsigned width_;
	unsigned height_;
};

#endif // SST_WINDOW_H