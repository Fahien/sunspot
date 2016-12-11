#ifndef SST_WAVEFRONTOBJECT_H
#define SST_WAVEFRONTOBJECT_H

#include <fstream>
#include <exception>
#include <vector>

#include "Graphics.h"
#include "Math.h"
#include "Mesh.h"


namespace sunspot {

struct Vertex;
class Texture;

class LoadingException : public std::runtime_error {
  public:
	LoadingException(const std::string &message) : std::runtime_error(message) {}
};


struct Face {
	int indices[4];
	int textures[4];
	int normals[4];
};


class WavefrontObject {
  public:
	WavefrontObject();
	~WavefrontObject();

	inline std::string &getName() { return name_; }
	inline Mesh *getMesh() { return meshes_.front(); }
	inline std::vector<Vertex> &getVertices() { return vertices_; }
	inline std::vector<GLuint> &getIndices() { return indices_; }
	inline std::vector<Texture> &getTextures() { return textures_; }

  private:
	void loadName(std::stringstream &is);
	void loadPosition(std::stringstream &is);
	void loadTexCoords(std::stringstream &is);
	void loadNormal(std::stringstream &is);
	void loadIndices(std::stringstream &is);
	void loadGroup(std::stringstream &is);
	void loadCachedMesh();

	friend std::ifstream &operator>>(std::ifstream &in, WavefrontObject &obj);

	std::string name_;

	std::string currentGroupName_;
	unsigned positionCount_;
	unsigned normalCount_;
	unsigned texCoordsCount_;

	std::vector<Vertex> vertices_;
	std::vector<math::Vec2> texCoords_;
	std::vector<math::Vec3> normals_;
	std::vector<GLuint> indices_;
	std::vector<Texture> textures_;

	std::vector<Mesh*> meshes_;
};

std::ifstream &operator>>(std::ifstream &in, WavefrontObject &obj);

}


#endif // SST_WAVEFRONTOBJECT_H

