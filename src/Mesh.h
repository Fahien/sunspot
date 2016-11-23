#ifndef SST_MESH_H
#define SST_MESH_H

#include "Graphics.h"
#include "Math.h"
#include <vector>

using namespace std;
class ShaderProgram;

namespace sunspot {


struct Vertex {
	math::Vec3 position;
	math::Vec3 normal;
	math::Vec3 texCoords;
};

enum TextureType { DIFFUSE, SPECULAR };
static const char *textureTypeNames[] = { "diffuse", "specular" };
const char *getTextureTypeName(const TextureType type) { return textureTypeNames[type]; }

struct Texture {
	~Texture() { glDeleteTextures(1, &id); }
	GLuint id;
	TextureType type;
};


class Mesh {
  public:
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	~Mesh();

	void draw(const ShaderProgram *shader);

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

  private:
	GLuint vao_;
	GLuint vbo_;
	GLuint ebo_;
};

}
#endif // SST_MESH_H