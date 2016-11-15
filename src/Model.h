#ifndef SST_MODEL_H
#define SST_MODEL_H

#include "Graphics.h"
#include "Math.h"

class ShaderProgram;

struct Material {
	math::Vec3 ambient;
	math::Vec3 diffuse;
	math::Vec3 specular;
	float shininess;
};


class Model {
  public:
	Model(const char *path, const float scale);
	~Model();

	inline void bind() const { glBindVertexArray(vao_); }
	inline void unbind() const { glBindVertexArray(0); }
	
	void render(const ShaderProgram *program) const;

	math::Mat4 transform;
  private:
	GLuint vbo_;
	GLuint ebo_;
	GLuint vao_;
	Material material_;
	GLuint texture_;
};

#endif // SST_MODEL_H