#ifndef SST_MODEL_H_
#define SST_MODEL_H_


#include <Gltf.h>

#include "view/GltfRenderer.h"

namespace gst = gltfspot;


namespace sunspot::component
{


class Model
{
  public:
	Model(gst::Gltf::Node& node, GltfRenderer& renderer)
	: mNode     { node     }
	, mRenderer { renderer }
	{}

	~Model() {}

	gst::Gltf::Node&  GetNode()     { return mNode;     }
	GltfRenderer&     GetRenderer() { return mRenderer; }

  private:
	gst::Gltf::Node  mNode;
	GltfRenderer&    mRenderer;
};


}

#endif // SST_MODEL_H_
