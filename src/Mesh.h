#ifndef SST_MESH_H
#define SST_MESH_H

#include <vector>
#include "Graphics.h"
#include "Math.h"
#include "Texture.h"

class ShaderProgram;

namespace sunspot {


struct Vertex {
	math::Vec3 position;
	math::Vec3 normal;
	math::Vec2 texCoords;
};


class Mesh {
  public:
	Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);
	~Mesh();

	void draw(const ShaderProgram *shader);

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

  private:
	GLuint vao_;
	GLuint vbo_;
	GLuint ebo_;
};


}

#endif // SST_MESH_H

