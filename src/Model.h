#ifndef SST_MODEL_H
#define SST_MODEL_H

#include "Graphics.h"

class Model
{
  public:
	Model();
	~Model();

	inline void bind() const { glBindVertexArray(vao_); }
	inline void unbind() const { glBindVertexArray(0); }

	inline void render() const { glDrawArrays(GL_TRIANGLES, 0, 3); }

  private:
	GLuint vbo_;
	GLuint vao_;
};

#endif // SST_MODEL_H
