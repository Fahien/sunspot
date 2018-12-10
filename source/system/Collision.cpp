#include "sunspot/entity/Entity.h"
#include "system/Collision.h"
#include "sunspot/component/Collider.h"
#include "sunspot/component/Transform.h"

using namespace sunspot;


void system::Collision::Add(Entity& entity)
{
	auto c = entity.GetCollider();
	// Check whether the entity has a collider
	if (c)
	{
		// Put a copy of the bounding box in the map
		auto pair = mBoxes.emplace(&entity, c->GetBoundingBox());

		// If it was successfull
		auto& success = pair.second;
		if (success)
		{
			auto& box = pair.first->second;

			// Set callback
			box.fStartCollidingWith = [&entity] (hst::BoundingBox *other) {
				entity.Collide(*reinterpret_cast<Entity*>(other->GetPayload()));
			};

			// Add that copy to the collision system
			mSystem.AddBox(&box);
		}
	}
}


void system::Collision::Update()
{
	// Update boxes
	for (auto& pair : mBoxes)
	{
		auto pTransform = pair.first->GetTransform();
		auto& box = pair.second;

		// Apply transform to the box
		// TODO rotation and scale
		// Fix Y-Z incoherency
		box.x = pTransform->position.x;
		box.y = pTransform->position.y;
	}

	mSystem.Update();
}
