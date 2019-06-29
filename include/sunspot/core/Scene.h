#ifndef SST_SCENE_H_
#define SST_SCENE_H_

#include <vector>

namespace sunspot
{
class Entity;

class Scene
{
  public:

	/// @param e Entity to add to the scene
	void add( Entity& e ) { entities.push_back( &e ); }

	/// @return The entities in the scene
	const std::vector<Entity*> &get_entities() const { return entities; }

  private:
	std::vector<Entity*> entities = {};
};

}  // namespace sunspot


#endif  // SST_SCENE_H_