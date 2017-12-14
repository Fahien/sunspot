#include "Entity.h"
#include "Mesh.h"
#include "Logger.h"

namespace sst = sunspot;


sst::Entity::Entity(sst::Mesh& mesh)
:	mMesh  { mesh }
,	mScript{ *this }
{
	sst::Logger::log.info("Entity: %s\n", mesh.GetName().c_str());
}


void sst::Entity::Update(const float delta)
{
	mScript.Update(delta);
}
