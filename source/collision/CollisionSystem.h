#ifndef SST_COLLISIONSYSTEM_H
#define SST_COLLISIONSYSTEM_H

#include <vector>

namespace sunspot
{

class BoundingBox;

class CollisionSystem
{
public:
	inline void addBox(BoundingBox *box) { boxes_.push_back(box); }

	void update();

  private:
	std::vector<BoundingBox *> boxes_;
};

}

#endif // SST_COLLISIONSYSTEM_H
