#include <Logger.h>

#include "Mesh.h"
#include "Entity.h"

using namespace sunspot;
namespace lst = logspot;


Entity::Entity()
:	mMesh{ nullptr }
,	mScript{ new Script{ *this } }
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


void Entity::Update(const float delta)
{
	if (mScript)
	{
		mScript->Update(delta);
	}
}
