#include "sunspot/entity/Entity.h"

#include "Mesh.h"
#include "sunspot/component/Collider.h"
#include "sunspot/component/Rigidbody.h"
#include "sunspot/component/Transform.h"

using namespace sunspot;


Entity::Entity( const int id, const std::string& name )
    : Object{ id, name }
{
}


Entity::~Entity()
{
	// It does not own the Model

	if ( mCollider )
	{
		delete mCollider;
	}

	if ( mScript )
	{
		delete mScript;
	}
}


// void Entity::SetModel(component::Model* model)
//{
//	if (mModel == model)
//	{
//		return;
//	}
//
//	mModel = model;
//}


void Entity::SetCollider( component::Collider* collider )
{
	if ( mCollider == collider )
	{
		return;
	}

	if ( mCollider )
	{
		delete mCollider;
	}

	mCollider = collider;
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
