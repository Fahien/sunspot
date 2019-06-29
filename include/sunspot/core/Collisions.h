#ifndef SST_COLLISIONS_H_
#define SST_COLLISIONS_H_

#include <unordered_map>

#include <hitspot/BoundingBox.h>
#include <hitspot/CollisionSystem.h>

namespace hst = hitspot;

namespace sunspot
{
class Entity;


class Collisions
{
  public:
	/// @param e Entity to add to the collision system
	void add( Entity& e );

	/// @brief Updates collisions
	void update();

  private:
	std::unordered_map<Entity*, hst::BoundingBox> boxes{};

	hst::CollisionSystem system{};
};


}  // namespace sunspot

#endif  // SST_COLLISIONS_H_
