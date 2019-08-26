#ifndef SST_GLTFMESH_H_
#define SST_GLTFMESH_H_

#include <vector>

#include <gltfspot/Gltf.h>

#include "view/GltfPrimitive.h"


namespace sunspot
{
class ShaderProgram;


class GltfMesh
{
  public:
	GltfMesh() = default;

	GltfMesh( GltfMesh&& ) = default;

	GltfMesh( gltfspot::Gltf& model, gltfspot::Mesh& mesh );

	/// @return The name
	const std::string& get_name() const { return name; }

	/// @return The list of primitives
	std::vector<GltfPrimitive>& get_primitives() { return primitives; }

	void draw( const graphics::shader::Program& shader, const mathspot::Mat4& transform = mathspot::Mat4::identity ) const;

  private:
	std::string                name{};
	std::vector<GltfPrimitive> primitives{};
};


}  // namespace sunspot


#endif  // SST_GLTFMESH_H_
