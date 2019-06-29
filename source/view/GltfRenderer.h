#pragma once

#include <unordered_map>

#include <mathspot/Math.h>
#include <gltfspot/Gltf.h>


#include "sunspot/system/graphics/Shader.h"
#include "view/GltfMesh.h"


namespace mst = mathspot;
namespace gst = gltfspot;


namespace sunspot
{

class GltfRenderer
{
  public:
	GltfRenderer(gst::Gltf&& gltf);
	GltfRenderer(GltfRenderer&& other);
	~GltfRenderer();

	gst::Gltf& GetGltf() { return m_Gltf; }

	void Draw(const graphics::shader::Program& shader);

	void Draw(const graphics::shader::Program& shader,
	          const gst::Gltf::Node* pNode,
	          const mst::Mat4& transform = mst::Mat4::identity);

  private:
	gst::Gltf m_Gltf;
	std::unordered_map<gst::Gltf::Mesh*, GltfMesh> m_Meshes = {};
};

} // namespace sunspot
