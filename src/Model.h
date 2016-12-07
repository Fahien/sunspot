#ifndef SST_MODEL_H
#define SST_MODEL_H

#include <string>
#include "Graphics.h"
#include "Math.h"
#include "Material.h"

class ShaderProgram;

class Model {
  public:
	Model(const std::string &path, const float scale);
	~Model();

	inline void bind() const { glBindVertexArray(vao_); }
	inline void unbind() const { glBindVertexArray(0); }
	
	void render(const ShaderProgram *program) const;

	math::Mat4 transform;
  private:
	GLuint vbo_;
	GLuint ebo_;
	GLuint vao_;
	sunspot::Material material_;
};

#endif // SST_MODEL_H

