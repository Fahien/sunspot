#ifndef SST_GLTFRENDERER_H_
#define SST_GLTFRENDERER_H_

#include "Graphics.h"
#include "Material.h"
#include "Texture.h"
#include "view/GltfMesh.h"

#include <Gltf.h>


namespace gst = gltfspot;


namespace sunspot
{

class ShaderProgram;

class GltfRenderer
{
public:
	GltfRenderer(gst::Gltf& model);
	~GltfRenderer();

	void Draw(const ShaderProgram& shader);

private:
	void draw(const ShaderProgram& shader, const gst::Gltf::Node* const pNode);

	gst::Gltf mModel;
	std::map<gst::Gltf::Mesh*, GltfMesh> mMeshes;

	GLuint mVao;
	std::map<size_t, GLuint> mVbos;
	GLuint mEbo;
	size_t mIndicesCount;
	bool mHasVertexColors;

	Material mMaterial;

	std::map<gst::Gltf::Texture*, Texture> mTextures;
};

}

#endif // SST_GLTFRENDERER_H_