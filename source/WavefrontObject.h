#ifndef SST_WAVEFRONTOBJECT_H
#define SST_WAVEFRONTOBJECT_H

#include <exception>
#include <vector>
#include <memory>
#include <mathspot/Math.h>
#include <filespot/Ifstream.h>

#include "sunspot/system/graphics/Gl.h"
#include "Mesh.h"

namespace fst = filespot;


namespace sunspot {

class  ShaderProgram;
struct Vertex;
class  Texture;
class  Mesh;


class LoadingException : public std::runtime_error
{
public:
	LoadingException(const std::string& message) : std::runtime_error(message) {}
};


struct Face
{
	int indices[4];
	int textures[4];
	int normals[4];
};


class WavefrontObject
{
public:
	WavefrontObject();
	~WavefrontObject();

	inline std::string& getName() { return mName; }
	inline std::vector<Vertex>&    getVertices()  { return mVertices;  }
	inline std::vector<GLuint>&    getIndices()   { return mIndices;   }
	inline std::vector<Texture>&   getTextures()  { return mTextures;  }
	inline std::vector<std::shared_ptr<graphics::Material>>& GetMaterials() { return mMaterials; }

	std::shared_ptr<Mesh> GetMesh(const std::string& name);
	inline std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return mMeshes; }

	void draw(const ShaderProgram& shader) const;

private:
	void loadName(std::stringstream& ss);
	void loadPosition(std::stringstream& ss);
	void loadTexCoords(std::stringstream& ss);
	void loadNormal(std::stringstream& ss);
	void loadIndices(std::stringstream& ss);
	void loadGroup(std::stringstream& ss);
	void loadCachedMesh();

	void createMaterial(std::stringstream& ss);
	void loadAmbient(std::stringstream& ss);
	void loadDiffuse(std::stringstream& ss);
	void loadSpecular(std::stringstream& ss);
	void loadAmbientMap(std::stringstream& ss, const std::string& path);
	void loadDiffuseMap(std::stringstream& ss, const std::string& path);
	void loadSpecularMap(std::stringstream& ss, const std::string& path);

	void loadMaterials(fst::Ifstream& is);
	void loadMaterialLibrary(std::stringstream& ss, const std::string& path);
	void useMaterial(std::stringstream& ss);
	void loadCachedMaterial();

	friend fst::Ifstream& operator>>(fst::Ifstream& is, WavefrontObject& obj);

	std::string mName;

	std::string mCurrentGroupName;
	unsigned mPositionCount;
	unsigned mNormalCount;
	unsigned mTexCoordsCount;
	unsigned mVertexCount;

	std::vector<mst::Vec3> mPositions;
	std::vector<mst::Vec2> mTexCoords;
	std::vector<mst::Vec3> mNormals;
	std::vector<Vertex>    mVertices;
	std::vector<GLuint>    mIndices;
	std::vector<Texture>   mTextures;

	std::vector<std::shared_ptr<Mesh>> mMeshes;
	
	std::shared_ptr<graphics::Material> mCurrentMaterial;
	std::vector<std::shared_ptr<graphics::Material>> mMaterials;
};

fst::Ifstream &operator>>(fst::Ifstream& is, WavefrontObject& obj);

}


#endif // SST_WAVEFRONTOBJECT_H
