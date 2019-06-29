#include "sunspot/entity/Entity.h"

#include "Mesh.h"
#include "sunspot/component/Collider.h"
#include "sunspot/component/Rigidbody.h"
#include "sunspot/component/Transform.h"

namespace sunspot
{
Entity::Entity( const int id, const std::string& name )
    : Object{ id, name }
{
}


Entity::~Entity()
{
	if ( mScript )
	{
		delete mScript;
	}
}


void Entity::SetTransform( component::Transform& transform )
{
	mTransform = transform;
}


void Entity::SetRigidbody( component::Rigidbody* rigidbody )
{
	if ( mRigidbody == rigidbody )
	{
		return;
	}

	if ( mRigidbody )
	{
		delete mRigidbody;
	}

	mRigidbody = rigidbody;
}


void Entity::Handle( const input::Input& input )
{
	if ( mScript )
	{
		mScript->handle( input );
	}
}


void Entity::Collide( Entity& other )
{
	if ( mScript )
	{
		mScript->collide( other );
	}
}


void Entity::Update( const float delta )
{
	if ( mScript )
	{
		mScript->update( delta );
	}
}


}  // namespace sunspot 
