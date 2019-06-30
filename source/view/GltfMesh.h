#ifndef SST_GLTFMESH_H_
#define SST_GLTFMESH_H_

#include <vector>

#include <gltfspot/Gltf.h>

#include "view/GltfPrimitive.h"

namespace gst = gltfspot;


namespace sunspot
{
class ShaderProgram;


class GltfMesh
{
  public:
	GltfMesh() = default;

	GltfMesh( GltfMesh&& other );

	GltfMesh( gst::Gltf& model, gst::Gltf::Mesh& mesh );

	/// @return The name
	const std::string& GetName() const { return mName; }

	/// @return The list of primitives
	std::vector<GltfPrimitive>& GetPrimitives() { return mPrimitives; }

	void Draw( const graphics::shader::Program& shader ) const;

  private:
	std::string                mName{};
	std::vector<GltfPrimitive> mPrimitives{};
};


}  // namespace sunspot


#endif  // SST_GLTFMESH_H_
