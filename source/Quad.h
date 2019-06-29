#pragma once

#include <logspot/Log.h>

#include "sunspot/graphics/Gl.h"


namespace sunspot
{


class Quad
{
  public:
	Quad();
	~Quad();

	inline void bind() const { glBindVertexArray(vao_); }
	inline void unbind() const { glBindVertexArray(0); }
	inline void render() const { glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); }

  private:
	GLuint vbo_;
	GLuint ebo_;
	GLuint vao_;
};


} // namespace sunspot
