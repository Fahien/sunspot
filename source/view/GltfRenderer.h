#ifndef SST_GLTFRENDERER_H_
#define SST_GLTFRENDERER_H_

#include <mathspot/Math.h>
#include <gltfspot/Gltf.h>

#include <unordered_map>

#include "Graphics.h"
#include "view/GltfMesh.h"


namespace mst = mathspot;
namespace gst = gltfspot;


namespace sunspot
{

class ShaderProgram;

class GltfRenderer
{
public:
	GltfRenderer(gst::Gltf&& gltf);
	GltfRenderer(GltfRenderer&& other);
	~GltfRenderer();

	gst::Gltf& GetGltf() { return m_Gltf; }

	void Draw(const ShaderProgram& shader);

	void Draw(const ShaderProgram& shader,
	          const gst::Gltf::Node* pNode,
	          const mst::Mat4& transform = mst::Mat4::identity);

private:
	gst::Gltf m_Gltf;
	std::unordered_map<gst::Gltf::Mesh*, GltfMesh> m_Meshes {};
};

}

#endif // SST_GLTFRENDERER_H_