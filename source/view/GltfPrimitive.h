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
	GltfPrimitive( gst::Gltf& model, gst::Mesh::Primitive& primitive );

	GltfPrimitive( gst::Gltf& model, gst::Shape& shape );

	/// Destructor
	~GltfPrimitive();

	void draw( const graphics::shader::Program& shader, const mst::Mat4& transform = mst::Mat4::identity ) const;

  private:
	void read_indices( gst::Gltf& model, gst::Mesh::Primitive& primitive );

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

	bool mHasVertexColors{};

	graphics::Material mMaterial{};

	std::map<gst::Gltf::Texture*, graphics::Texture> mTextures{};
};


}  // namespace sunspot


#endif  // SST_GLTPRIMITIVE_H_
