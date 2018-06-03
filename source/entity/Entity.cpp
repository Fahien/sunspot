#include <Logger.h>

#include "Mesh.h"
#include "Entity.h"

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


void Entity::Update(const float delta, input::Key key)
{
	if (mScript)
	{
		mScript->Update(delta, key);
	}
}
