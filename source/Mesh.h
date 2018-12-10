#ifndef SST_MESH_H_
#define SST_MESH_H_

#include <vector>
#include <memory>

#include <mathspot/Math.h>

#include "sunspot/system/graphic/Shader.h"
#include "sunspot/system/graphic/Texture.h"
#include "sunspot/system/graphic/Material.h"

namespace mst = mathspot;

namespace sunspot
{


namespace component
{

class Transform;

}


struct Vertex
{
	mst::Vec3 position;
	mst::Vec3 normal;
	mst::Vec2 texCoords;
};


class Mesh
{
public:
	Mesh(const std::string&         name,
	     std::vector<Vertex>&       vertices,
	     std::vector<GLuint>&       indices,
	     std::shared_ptr<graphic::Material>& material);
	~Mesh();

	/// Returns the name of the mesh
	std::string& GetName() { return mName; }

	/// Applies the value of the transform component to the member transform matrix
	void ApplyTransform(component::Transform& pTransform);

	void Draw(const graphic::shader::Program& shader) const;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<graphic::Texture> textures;

	mst::Mat4 transform;

private:
	std::string mName;
	GLuint mVao;
	GLuint mVbo;
	GLuint mEbo;
	std::shared_ptr<graphic::Material> mMaterial;
};


}


#endif // SST_MESH_H_
