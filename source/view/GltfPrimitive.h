#ifndef SST_GLTPRIMITIVE_H_
#define SST_GLTPRIMITIVE_H_

#include <gltfspot/Gltf.h>

#include "sunspot/graphics/Gl.h"
#include "sunspot/graphics/Material.h"
#include "sunspot/graphics/Shader.h"
#include "sunspot/graphics/Texture.h"


namespace mst = mathspot;
namespace gst = gltfspot;

namespace sunspot
{
class GltfPrimitive
{
  public:
	/// Base constructor
	GltfPrimitive() = default;

	/// Move constructor
	GltfPrimitive( GltfPrimitive&& other );

	/// Normal constructor
	GltfPrimitive( gst::Gltf& model, gst::Gltf::Mesh::Primitive& primitive );

	/// Destructor
	~GltfPrimitive();

	void SetMatrix( const mst::Mat4& matrix );

	void Draw( const graphics::shader::Program& shader ) const;

  private:
	void readIndices( gst::Gltf& model, gst::Gltf::Mesh::Primitive& primitive );

	bool                     mHasVao{};
	GLuint                   mVao{};
	std::map<size_t, GLuint> mVbos{};
	bool                     mHasEbo{};
	GLuint                   mEbo{};

	GLenum      mMode{};
	GLsizei     mIndicesCount = 0;
	GLenum      mIndicesType{};
	const void* mIndicesOffset{};

	GLsizei vertex_count = 0;

	mst::Mat4 mMatrix{};

	bool mHasVertexColors{};

	graphics::Material mMaterial{};

	std::map<gst::Gltf::Texture*, graphics::Texture> mTextures{};
};


}  // namespace sunspot


#endif  // SST_GLTPRIMITIVE_H_
