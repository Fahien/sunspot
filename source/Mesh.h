#ifndef SST_MESH_H
#define SST_MESH_H

#include <vector>
#include <MathSpot.h>

#include "Graphics.h"
#include "Texture.h"
#include "Material.h"

namespace mst = mathspot;

namespace sunspot {


class ShaderProgram;


struct Vertex
{
	mst::Vec3 position;
	mst::Vec3 normal;
	mst::Vec2 texCoords;
};


class Mesh
{
public:
	Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material* m);
	~Mesh();

	/// Returns the name of the mesh
	std::string& GetName() { return mName; }

	void Draw(const ShaderProgram& shader) const;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	mst::Mat4 transform;

private:
	std::string mName;
	GLuint mVao;
	GLuint mVbo;
	GLuint mEbo;
	Material* mMaterial;
};


}


#endif // SST_MESH_H
