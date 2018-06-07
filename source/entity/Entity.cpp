#include <Logger.h>

#include "Entity.h"

#include "Mesh.h"
#include "sunspot/component/Transform.h"

using namespace sunspot;
namespace lst = logspot;


Entity::Entity()
:	mScript{ new Script{ *this } }
{}


Entity::Entity(int id, std::string& name)
:	Object{ id, name }
{}


Entity::~Entity()
{
	if (mTransform)
	{
		delete mTransform;
	}

	if (mScript)
	{
		delete mScript;
	}
}


void Entity::SetTransform(component::Transform* transform)
{
	if (mTransform == transform)
	{
		return;
	}

	if (mTransform)
	{
		delete mTransform;
	}

	mTransform = transform;
}


void Entity::Update(const float delta, input::Key key)
{
	if (mScript)
	{
		mScript->Update(delta, key);
	}
}
