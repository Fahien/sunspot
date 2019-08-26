#ifndef SST_GLTPRIMITIVE_H_
#define SST_GLTPRIMITIVE_H_

#include <gltfspot/Gltf.h>

#include "sunspot/graphics/Gl.h"
#include "sunspot/graphics/Material.h"
#include "sunspot/graphics/Shader.h"
#include "sunspot/graphics/Texture.h"


namespace sunspot
{
class GltfPrimitive
{
  public:
	/// Base constructor
	GltfPrimitive() = default;

	GltfPrimitive( const GltfPrimitive& ) = delete;

	/// Move constructor
	GltfPrimitive( GltfPrimitive&& other );

	/// Move assignment
	GltfPrimitive& operator=( GltfPrimitive&& other );

	/// Normal constructor
	GltfPrimitive( gltfspot::Gltf& model, gltfspot::Mesh::Primitive& primitive );

	GltfPrimitive( gltfspot::Gltf& model, gltfspot::Shape& shape );

	/// Destructor
	~GltfPrimitive();

	void bind_material( const graphics::shader::Program& shader, gltfspot::Material& material ) const;

	void draw( const graphics::shader::Program& shader, const mathspot::Mat4& transform = mathspot::Mat4::identity ) const;

  private:
	void read_indices( gltfspot::Gltf& model, gltfspot::Mesh::Primitive& primitive );

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

	gltfspot::Mesh::Primitive* primitive = nullptr;

	graphics::Material mMaterial{};

	std::unordered_map<gltfspot::Texture*, graphics::Texture> mTextures{};
};


}  // namespace sunspot


#endif  // SST_GLTPRIMITIVE_H_
