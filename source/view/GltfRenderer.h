#ifndef SST_GLTFRENDERER_H_
#define SST_GLTFRENDERER_H_

#include "Graphics.h"
#include "Material.h"

#include <Gltf.h>


namespace gsp = gltfspot;


namespace sunspot
{

class ShaderProgram;

class GltfRenderer
{
public:
	GltfRenderer(gsp::Gltf& model);
	~GltfRenderer();

	void Draw(const ShaderProgram& shader) const;

private:
	GLuint mVao;
	GLuint mVbo;
	GLuint mEbo;
	size_t mIndicesCount;

	Material mMaterial;
};

}

#endif // SST_GLTFRENDERER_H_