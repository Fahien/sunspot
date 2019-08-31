#pragma once

#include <unordered_map>

#include <gltfspot/Gltf.h>
#include <mathspot/Math.h>


#include "sunspot/graphics/Shader.h"
#include "view/GltfCamera.h"
#include "view/GltfMesh.h"


namespace mst = mathspot;
namespace gst = gltfspot;


namespace sunspot
{
class GltfRenderer
{
  public:
	GltfRenderer( gst::Gltf* g = nullptr );

	void set_gltf( gst::Gltf& g );

	gst::Gltf* get_gltf() { return gltf; }

	void draw( const graphics::shader::Program& shader );

	void draw( const graphics::shader::Program& shader, const gst::Node& node,
	           const mst::Mat4& transform = mst::Mat4::identity );

	void draw( const graphics::shader::Program& shader, const gst::Camera& camera,
	           const mst::Mat4& transform = mst::Mat4::identity );

	void draw( const graphics::shader::Program& shader, const gst::Light& light, const mst::Mat4& transform );


  private:
	// TODO either GLTF and GLTF Meshes should go somewhere else
	gst::Gltf*                                                    gltf    = {};
	std::unordered_map<gst::Mesh*, GltfMesh>                      meshes  = {};
	std::vector<GltfPrimitive>                                    shapes  = {};
	std::unordered_map<gst::Camera*, std::unique_ptr<GltfCamera>> cameras = {};

	graphics::shader::Program shape_shader = { "shader/shape.vert", "shader/shape.frag" };

  public:
	bool render_bounds = false;
};

}  // namespace sunspot
