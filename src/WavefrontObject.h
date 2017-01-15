#ifndef SST_WAVEFRONTOBJECT_H
#define SST_WAVEFRONTOBJECT_H

#include <exception>
#include <vector>

#include "Graphics.h"
#include "Math.h"
#include "Mesh.h"
#include "Ifstream.h"


class ShaderProgram;


namespace sunspot {

struct Vertex;
class Texture;
class Mesh;

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
	inline std::vector<Vertex> &getVertices() { return vertices_; }
	inline std::vector<GLuint> &getIndices() { return indices_; }
	inline std::vector<Texture> &getTextures() { return textures_; }

	void draw(const ShaderProgram *shader) const;

  private:
	void loadName(std::stringstream &ss);
	void loadPosition(std::stringstream &ss);
	void loadTexCoords(std::stringstream &ss);
	void loadNormal(std::stringstream &ss);
	void loadIndices(std::stringstream &ss);
	void loadGroup(std::stringstream &ss);
	void loadCachedMesh();

	void createMaterial(std::stringstream &ss);
	void loadAmbient(std::stringstream &ss);
	void loadDiffuse(std::stringstream &ss);
	void loadSpecular(std::stringstream &ss);
	void loadAmbientMap(std::stringstream &ss, const std::string &path);
	void loadDiffuseMap(std::stringstream &ss, const std::string &path);
	void loadSpecularMap(std::stringstream &ss, const std::string &path);

	void loadMaterials(Ifstream &is);
	void loadMaterialLibrary(std::stringstream &ss, const std::string &path);
	void useMaterial(std::stringstream &ss);
	void loadCachedMaterial();

	friend Ifstream &operator>>(Ifstream &is, WavefrontObject &obj);

	std::string name_;

	std::string currentGroupName_;
	unsigned positionCount_;
	unsigned normalCount_;
	unsigned texCoordsCount_;
	unsigned vertexCount_;

	std::vector<math::Vec3> positions_;
	std::vector<math::Vec2> texCoords_;
	std::vector<math::Vec3> normals_;
	std::vector<Vertex> vertices_;
	std::vector<GLuint> indices_;
	std::vector<Texture> textures_;
	std::vector<Mesh *> meshes_;
	
	Material *currentMaterial_;
	std::vector<Material *> materials_;
};

}


#endif // SST_WAVEFRONTOBJECT_H
