#ifndef SST_MESH_H
#define SST_MESH_H

#include <vector>
#include "Graphics.h"
#include "Math.h"
#include "Texture.h"
#include "Material.h"
#include "WavefrontObject.h"

class ShaderProgram;


namespace sunspot {


struct Vertex {
  public:
	math::Vec3 position;
	math::Vec3 normal;
	math::Vec2 texCoords;
};


class Mesh {
  public:
	Mesh(const std::string &name, std::vector<Vertex> &vertices, std::vector<GLuint> &indices, Material *m);
	~Mesh();

	void draw(const ShaderProgram *shader);

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	math::Mat4 transform;

  private:
	std::string name_;

	GLuint vao_;
	GLuint vbo_;
	GLuint ebo_;
	Material *material_;
};


}

#endif // SST_MESH_H
