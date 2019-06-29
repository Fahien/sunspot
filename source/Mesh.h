#ifndef SST_MESH_H_
#define SST_MESH_H_

#include <vector>
#include <memory>

#include <mathspot/Math.h>

#include "sunspot/system/graphics/Shader.h"
#include "sunspot/system/graphics/Texture.h"
#include "sunspot/system/graphics/Material.h"

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
	     std::shared_ptr<graphics::Material>& material);
	~Mesh();

	/// Returns the name of the mesh
	std::string& GetName() { return mName; }

	/// Applies the value of the transform component to the member transform matrix
	void ApplyTransform(component::Transform& pTransform);

	void Draw(const graphics::shader::Program& shader) const;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<graphics::Texture> textures;

	mst::Mat4 transform;

private:
	std::string mName;
	GLuint mVao;
	GLuint mVbo;
	GLuint mEbo;
	std::shared_ptr<graphics::Material> mMaterial;
};


}


#endif // SST_MESH_H_
