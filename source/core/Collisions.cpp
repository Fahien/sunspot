#include "sunspot/core/Collisions.h"
#include "sunspot/component/Collider.h"
#include "sunspot/component/Transform.h"
#include "sunspot/entity/Entity.h"

namespace sunspot
{
void Collisions::add( Entity& entity )
{
	// Check whether the entity has a collider
	if ( auto c = entity.get<component::Collider>() )
	{
		// Put a copy of the bounding box in the map
		auto pair = boxes.emplace( &entity, c->GetBoundingBox() );

		// If it was successfull
		auto& success = pair.second;
		if ( success )
		{
			auto& box = pair.first->second;

			// Set callback
			box.fStartCollidingWith = [&entity]( hst::BoundingBox* other ) {
				entity.Collide( *reinterpret_cast<Entity*>( other->GetPayload() ) );
			};

			// Add that copy to the collision system
			system.AddBox( &box );
		}
	}
}


void Collisions::update()
{
	// Update boxes
	for ( auto& pair : boxes )
	{
		if ( auto transform = pair.first->get<component::Transform>() )
		{
			auto& box = pair.second;

			// Apply transform to the box
			// TODO rotation and scale
			// Fix Y-Z incoherency
			box.x = transform->position.x;
			box.y = transform->position.y;
		}
	}

	system.Update();
}


}  // namespace sunspot
