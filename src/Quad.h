#ifndef SST_QUAD_H
#define SST_QUAD_H

#include "Graphics.h"

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

#endif // SST_QUAD_H
