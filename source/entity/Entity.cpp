#include "Entity.h"
#include "Mesh.h"
#include "Logger.h"

namespace sst = sunspot;


sst::Entity::Entity()
:	mScript{ new sst::Script{ *this } }
{}


sst::Entity::Entity(sst::Mesh* mesh)
:	mMesh     { mesh }
,	mTransform{ new PySpotTransform{} }
,	mScript   { new sst::Script{ *this } }
{
	sst::Logger::log.info("Entity: %s\n", mesh->GetName().c_str());
}


sst::Entity::Entity(int id, std::string& name)
:	sst::Object{ id, name }
{}


sst::Entity::~Entity()
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


void sst::Entity::Update(const float delta)
{
	mScript->Update(delta);
}
