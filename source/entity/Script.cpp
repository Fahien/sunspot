#include <PySpotTuple.h>
#include <PySpotString.h>
#include <Logger.h>
#include <PySpotExtension.h>

#include "Script.h"
#include "Entity.h"
#include "Mesh.h"

using namespace sunspot;
namespace mst = mathspot;
namespace lst = logspot;


pst::PySpot Script::pyspot{ "sunspot", PyInit_PySpot };


Script::Script(Entity& entity)
:	mEntity{ entity }
,	mModule{ pyspot.ImportModule(entity.GetMesh()->GetName().c_str()) }
,	mArgs  { 2 }
{
	pst::PySpotTuple args{ 1 };
	args.SetItem(0, *mEntity.mTransform);

	lst::Logger::log.info("Calling python init\n");
	mModule.CallFunction("init", args);

	mEntity.mMesh->transform.ScaleX(mEntity.mTransform->GetScale().GetX());
	mEntity.mMesh->transform.ScaleY(mEntity.mTransform->GetScale().GetY());
	mEntity.mMesh->transform.ScaleZ(mEntity.mTransform->GetScale().GetZ());
	mEntity.mMesh->transform.TranslateX(mEntity.mTransform->GetPosition().GetX());
	mEntity.mMesh->transform.TranslateY(mEntity.mTransform->GetPosition().GetY());
	mEntity.mMesh->transform.TranslateZ(mEntity.mTransform->GetPosition().GetZ());
	mEntity.mMesh->transform.RotateX(mEntity.mTransform->GetRotation().GetX());
	mEntity.mMesh->transform.RotateY(mEntity.mTransform->GetRotation().GetY());
	mEntity.mMesh->transform.RotateZ(mEntity.mTransform->GetRotation().GetZ());
}


void Script::Update(const float delta)
{
	mArgs.SetItem(0, delta);
	mArgs.SetItem(1, *mEntity.mTransform);
	
	lst::Logger::log.info("Calling python update\n");
	mModule.CallFunction("update", mArgs);
	
	mEntity.mMesh->transform = mst::Mat4::identity;
	mEntity.mMesh->transform.ScaleX(mEntity.mTransform->GetScale().GetX());
	mEntity.mMesh->transform.ScaleY(mEntity.mTransform->GetScale().GetY());
	mEntity.mMesh->transform.ScaleZ(mEntity.mTransform->GetScale().GetZ());
	mEntity.mMesh->transform.TranslateX(mEntity.mTransform->GetPosition().GetX());
	mEntity.mMesh->transform.TranslateY(mEntity.mTransform->GetPosition().GetY());
	mEntity.mMesh->transform.TranslateZ(mEntity.mTransform->GetPosition().GetZ());
	mEntity.mMesh->transform.RotateX(mEntity.mTransform->GetRotation().GetX());
	mEntity.mMesh->transform.RotateY(mEntity.mTransform->GetRotation().GetY());
	mEntity.mMesh->transform.RotateZ(mEntity.mTransform->GetRotation().GetZ());
}
