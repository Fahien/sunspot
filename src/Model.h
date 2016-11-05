#ifndef SST_MODEL_H
#define SST_MODEL_H

#include "Graphics.h"
#include "ShaderProgram.h"
#include "Math.h"

class Model
{
  public:
	Model();
	~Model();

	void translateX(const float amount);
	void rotateY(const float radians);
	void rotateZ(const float radians);

	inline void bind() const { glBindVertexArray(vao_); }
	inline void unbind() const { glBindVertexArray(0); }
	inline void render(const ShaderProgram &program) const
	{
		GLuint transformLoc {program.getLocation("model")};
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform_.matrix_);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

  private:
	GLuint vbo_;
	GLuint ebo_;
	GLuint vao_;
	math::Mat4 transform_;
};

#endif // SST_MODEL_H
