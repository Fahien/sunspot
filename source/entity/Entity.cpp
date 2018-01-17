#include "Entity.h"
#include "Mesh.h"
#include "Logger.h"

namespace sst = sunspot;


sst::Entity::Entity()
:	mScript{ *this }
{}


sst::Entity::Entity(sst::Mesh* mesh)
:	mMesh  { mesh }
,	mScript{ *this }
{
	sst::Logger::log.info("Entity: %s\n", mesh->GetName().c_str());
	mTransform.GetScale().GetComponent()->x = 1.0f;
	mTransform.GetScale().GetComponent()->y = 1.0f;
	mTransform.GetScale().GetComponent()->z = 1.0f;
}


void sst::Entity::Update(const float delta)
{
	mScript.Update(delta);
}
