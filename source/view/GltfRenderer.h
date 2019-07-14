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
	GltfRenderer( GltfRenderer&& other );
	~GltfRenderer();

	void set_gltf( gst::Gltf& g );

	gst::Gltf* get_gltf() { return m_Gltf; }

	void draw( const graphics::shader::Program& shader );

	void draw( const graphics::shader::Program& shader, const gst::Node& node,
	           const mst::Mat4& transform = mst::Mat4::identity );

	void draw( const graphics::shader::Program& shader, const gst::Camera& camera,
	           const mst::Mat4& transform = mst::Mat4::identity );

	void draw( const graphics::shader::Program& shader, const gst::Light& light, const mst::Mat4& transform );

  private:
	// TODO either GLTF and GLTF Meshes should go somewhere else
	gst::Gltf*                                                          m_Gltf   = {};
	std::unordered_map<gst::Mesh*, GltfMesh>                      m_Meshes = {};
	std::unordered_map<gst::Camera*, std::unique_ptr<GltfCamera>> cameras  = {};
};

}  // namespace sunspot
