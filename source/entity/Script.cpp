#include <PySpotTuple.h>
#include <PySpotString.h>

#include "Script.h"
#include "PySpotExtension.h"
#include "Logger.h"
#include "Entity.h"
#include "Mesh.h"

namespace sst = sunspot;
namespace mst = mathspot;


pst::PySpot sst::Script::pyspot{ "sunspot", PyInit_PySpot };


sst::Script::Script(sst::Entity& entity)
:	mEntity{ entity }
,	mModule{ pyspot.ImportModule(entity.GetMesh()->GetName().c_str()) }
{
	pst::PySpotTuple args{ 1 };
	args.SetItem(0, mEntity.mTransform);

	sst::Logger::log.info("Calling python init\n");
	mModule.CallFunction("init", args);

	mEntity.mMesh->transform = mst::Mat4::identity;
	mEntity.mMesh->transform.translateX(mEntity.mTransform.GetX());
	mEntity.mMesh->transform.translateY(mEntity.mTransform.GetY());
	mEntity.mMesh->transform.translateZ(mEntity.mTransform.GetZ());
}


void sst::Script::Update(const float delta)
{
	pst::PySpotTuple args{ 2 };
	args.SetItem(0, delta);
	args.SetItem(1, mEntity.mTransform);
	
	sst::Logger::log.info("Calling python update\n");
	mModule.CallFunction("update", args);
	
	//mEntity.mMesh->transform = mst::Mat4::identity;
	//mEntity.mMesh->transform.rotateX(mEntity.mTransform.GetX());
	//mEntity.mMesh->transform.rotateY(mEntity.mTransform.GetY());
	//mEntity.mMesh->transform.rotateZ(mEntity.mTransform.GetZ());
}
