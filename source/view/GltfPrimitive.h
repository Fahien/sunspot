#ifndef SST_GLTPRIMITIVE_H_
#define SST_GLTPRIMITIVE_H_

#include "Graphics.h"
#include "Material.h"
#include "Texture.h"

#include <gltfspot/Gltf.h>

namespace mst = mathspot;
namespace gst = gltfspot;

namespace sunspot
{


class ShaderProgram;


class GltfPrimitive
{
public:
	/// Base constructor
	GltfPrimitive(){}

	/// Move constructor
	GltfPrimitive(GltfPrimitive&& other);

	/// Normal constructor
	GltfPrimitive(gst::Gltf& model, gst::Gltf::Mesh::Primitive& primitive);

	/// Destructor
	~GltfPrimitive();

	void SetMatrix(const mst::Mat4& matrix);

	void Draw(const ShaderProgram& shader) const;

private:
	void readIndices(gst::Gltf& model, gst::Gltf::Mesh::Primitive& primitive);

	bool mHasVao{};
	GLuint mVao{};
	std::map<size_t, GLuint> mVbos{};
	bool mHasEbo{};
	GLuint mEbo{};

	GLenum  mMode{};
	GLsizei mIndicesCount{};
	GLenum  mIndicesType{};
	const void* mIndicesOffset{};

	mst::Mat4 mMatrix{};

	bool mHasVertexColors{};

	Material mMaterial{};

	std::map<gst::Gltf::Texture*, Texture> mTextures{};

};


}


#endif // SST_GLTPRIMITIVE_H_
