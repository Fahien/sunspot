#ifndef SST_EDITOR_H_
#define SST_EDITOR_H_

#include <gltfspot/Gltf.h>

namespace sunspot
{
class Game;

class Editor
{
  public:
	Editor( Game& g )
	    : game{ g }
	{
	}

	void draw( gltfspot::Gltf& gltf );

  private:
	void draw_transform( gltfspot::Node& node );

	void draw( gltfspot::Shape& bounds );

	void draw( gltfspot::Camera& camera );

	void draw( gltfspot::Light& light );

	void draw( gltfspot::Mesh& mesh );

	void draw( gltfspot::Node& node );

	Game& game;

	/// Keep track of selected node
	void* selected = nullptr;

	/// Keep track of current gltf being drawn
	gltfspot::Gltf* gltf = nullptr;
};


}  // namespace sunspot

#endif  // SST_EDITOR_H_
