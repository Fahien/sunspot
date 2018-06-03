#ifndef SST_GLTFRENDERER_H_
#define SST_GLTFRENDERER_H_

#include <MathSpot.h>
#include <Gltf.h>

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

	gst::Gltf& GetGltf() { return mGltf; }

	void Draw(const ShaderProgram& shader);

	void Draw(const ShaderProgram& shader,
	          const gst::Gltf::Node* pNode,
	          const mst::Mat4& transform = mst::Mat4::identity);

private:
	gst::Gltf mGltf;
	std::map<gst::Gltf::Mesh*, GltfMesh> mMeshes {};
};

}

#endif // SST_GLTFRENDERER_H_