#ifndef SST_COLLISION_H
#define SST_COLLISION_H

#include <unordered_map>
#include <hitspot/CollisionSystem.h>
#include <hitspot/BoundingBox.h>

namespace hst = hitspot;


namespace sunspot
{

class Entity;

namespace system
{

class Collision
{
public:
	void Add(Entity& e);

	void Update();

  private:
	std::unordered_map<Entity*, hst::BoundingBox> mBoxes;
	hst::CollisionSystem mSystem;
};

}

}

#endif // SST_COLLISION_H
