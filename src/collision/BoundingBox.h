#ifndef SST_BOUNDINGBOX_H
#define SST_BOUNDINGBOX_H

#include <vector>
#include <functional>
#include "../Math.h"


namespace sunspot
{

class BoundingBox : public math::Rectangle
{
public:
	BoundingBox();

	/// Returns the current collisions
	const std::vector<const BoundingBox *> &getCollisions() const { return collisions_; }

	/// Adds a Box to the collision list
	void addCollision(const BoundingBox *);

	/// Removes a Box from the collision list
	void removeCollision(const BoundingBox *);

	/// Tests whether a Box is in the collisions list
	bool isCollidingWith(const BoundingBox *box) const;

	std::function<void(BoundingBox *box)> startCollidingWith;
	std::function<void(BoundingBox *box)> collidingWith;
	std::function<void(BoundingBox *box)> endCollidingWith;

  private:
	std::vector<const BoundingBox *> collisions_;
};

}

#endif // SST_BOUNDINGBOX_H
