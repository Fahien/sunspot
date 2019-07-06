#ifndef SST_EDITOR_H_
#define SST_EDITOR_H_

#include <gltfspot/Gltf.h>

namespace sunspot
{
class Editor
{
  public:
	void draw( gltfspot::Gltf& gltf );

  private:
	void draw( gltfspot::Gltf::Node& node );

	/// Keep track of selected node
	void* selected = nullptr;

	/// Keep track of current gltf being drawn
	gltfspot::Gltf* gltf = nullptr;
};


}  // namespace sunspot

#endif  // SST_EDITOR_H_
