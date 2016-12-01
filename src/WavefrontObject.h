#ifndef SST_WAVEFRONTOBJECT_H
#define SST_WAVEFRONTOBJECT_H

#include <fstream>
#include <exception>
#include <vector>

#include "Graphics.h"
#include "Mesh.h"
#include "Material.h"


namespace sunspot {


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

	friend std::ifstream &operator>>(std::ifstream &in, WavefrontObject &obj);
	void loadPosition(std::stringstream is);
	void loadTexCoords(std::stringstream is);
	void loadNormal(std::stringstream is);
	void loadIndices(std::stringstream is);

  private:
	unsigned positionCount_;
	unsigned normalCount_;
	unsigned texCoordsCount_;

	std::vector<Vertex> vertices_;
	std::vector<GLuint> indices_;
	std::vector<Texture> textures_;
};

}

#endif // SST_WAVEFRONTOBJECT_H

