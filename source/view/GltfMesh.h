#ifndef SST_GLTFMESH_H_
#define SST_GLTFMESH_H_

#include "Graphics.h"
#include "Material.h"
#include "Texture.h"

#include <Gltf.h>

namespace mst = mathspot;
namespace gst = gltfspot;

namespace sunspot
{


class ShaderProgram;


class GltfMesh
{
public:
	/// Base constructor
	GltfMesh(){}

	/// Move constructor
	GltfMesh(GltfMesh&& other);

	/// Normal constructor
	GltfMesh(gst::Gltf& model, gst::Gltf::Mesh& mesh);

	/// Destructor
	~GltfMesh();

	void SetMatrix(const mst::Mat4& matrix);

	void Draw(const ShaderProgram& shader) const;

private:
	bool mHasVao;
	GLuint mVao;
	std::map<size_t, GLuint> mVbos;
	bool mHasEbo;
	GLuint mEbo;
	size_t mIndicesCount;

	mst::Mat4 mMatrix;

	bool mHasVertexColors;

	Material mMaterial;

	std::map<gst::Gltf::Texture*, Texture> mTextures;

};


}


#endif // SST_GLTFMESH_H_
