#include "CollisionSystem.h"
#include "BoundingBox.h"

using namespace sunspot;


void CollisionSystem::update()
{
	for (size_t i{ 0 }; i < boxes_.size(); ++i)
	{
		BoundingBox *box{ boxes_[i] };

		for (size_t j{ i + 1 }; j < boxes_.size(); ++j)
		{
			BoundingBox *otherBox{ boxes_[j] };
			bool isColliding{ box->isCollidingWith(otherBox) };
			if (box->intersects(otherBox))
			{
				if (!isColliding)
				{
					box->addCollision(otherBox);
					if (box->startCollidingWith)
						box->startCollidingWith(otherBox);
				}
				if (box->collidingWith)
					box->collidingWith(otherBox);
			}
			else
				if (isColliding)
				{
					box->removeCollision(otherBox);
					if (box->endCollidingWith)
						box->endCollidingWith(otherBox);
				}
		}
	}
}
