#pragma once

#include <gltfspot/Gltf.h>

#include "sunspot/entity/Object.h"
#include "view/GltfRenderer.h"

namespace gst = gltfspot;


namespace sunspot::component
{


class Model : public Object
{
  public:
	Model( gst::Gltf::Node& node, GltfRenderer& renderer )
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


} // namespace sunspot::component
