#include "BoundingBox.h"

using namespace sunspot;


BoundingBox::BoundingBox()
{
}


bool BoundingBox::isCollidingWith(const BoundingBox *box) const
{
	for (const BoundingBox *b : collisions_)
		if (box == b) return true;
	return false;
}


void BoundingBox::addCollision(const BoundingBox *box)
{
	collisions_.push_back(box);
}


void BoundingBox::removeCollision(const BoundingBox *box)
{
	for (size_t i{ 0 }; i < collisions_.size(); ++i)
		if (collisions_[i] == box)
		{
			collisions_.erase(collisions_.begin() + i);
			return;
		}
}