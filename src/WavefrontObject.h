#ifndef SST_WAVEFRONTOBJECT_H
#define SST_WAVEFRONTOBJECT_H

#include <fstream>
#include <exception>
#include <vector>

#include "Graphics.h"
#include "Math.h"
#include "Material.h"

class VertexLoadingException : public std::runtime_error {
  public:
	VertexLoadingException(const std::string &message) : std::runtime_error(message) {}
};


class TextureCoordinateLoadingException : public std::runtime_error {
  public:
	TextureCoordinateLoadingException(const std::string &message) : std::runtime_error(message) {}
};


class VertexNormalLoadingException : public std::runtime_error {
  public:
	VertexNormalLoadingException(const std::string &message) : std::runtime_error(message) {}
};


class FaceLoadingException : public std::runtime_error {
  public:
	FaceLoadingException(const std::string &message) : std::runtime_error(message) {};
};


struct Vertex {
  public:
	float x;
	float y;
	float z;
	float w;
};


struct TextureCoordinate {
  public:
	float u;
	float v;
};


struct VertexNormal {
  public:
	float i;
	float j;
	float k;
};


struct Face {
  public:
	int indices[4];
	int textures[4];
	int normals[4];
};


class WavefrontObject {
  public:
	WavefrontObject();
	~WavefrontObject();

	void initialize();

	inline void addVertex(const Vertex v) { vertices_.push_back(v); }
	inline void addTextureCoordinate(const TextureCoordinate t) { textureCoordinates_.push_back(t); }
	inline void addVertexNormal(const VertexNormal n) { normals_.push_back(n); }
	inline void addFace(const Face f) { faces_.push_back(f); }

	friend std::ifstream &operator>>(std::ifstream &in, WavefrontObject &obj);

	math::Math4 transform;

  private:
	std::vector<Vertex> vertices_;
	std::vector<TextureCoordinate> textureCoordinates_;
	std::vector<VertexNormal> normals_;
	std::vector<Face> faces_;

	GLuint vbo_;
	GLuint ebo_;
	GLuint vao_;
	Material material_;
};

#endif // SST_WAVEFRONTOBJECT_H

