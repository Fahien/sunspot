#include <Logger.h>

#include "Mesh.h"
#include "Entity.h"

using namespace sunspot;
namespace lst = logspot;


Entity::Entity()
:	mScript{ new Script{ *this } }
{}


Entity::Entity(Mesh* mesh)
:	mMesh     { mesh }
,	mTransform{ new PySpotTransform{} }
,	mScript   { new Script{ *this } }
{
	lst::Logger::log.info("Entity: %s\n", mesh->GetName().c_str());
}


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
	mScript->Update(delta);
}
