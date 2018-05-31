#include <Logger.h>
#include <pyspot/Tuple.h>
#include <pyspot/String.h>
#include <pyspot/extension/Extension.h>

#include "Script.h"
#include "Entity.h"
#include "Mesh.h"

using namespace sunspot;
using namespace pyspot;
namespace mst = mathspot;
namespace lst = logspot;


std::unique_ptr<Interpreter> Script::interpreter{ nullptr };


void Script::Initialize(const std::wstring& scriptPath)
{
	if (!interpreter)
	{
		interpreter = std::make_unique<Interpreter>("sunspot", PyInit_PySpot, scriptPath.c_str());
	}
}


Script::Script(const int id, std::string& name, Entity& entity)
:	Object { id, name }
,	mEntity{ entity }
,	mModule{ interpreter->ImportModule(name) }
,	mArgs  { 2 }
{}


void Script::Initialize()
{
	if (mEntity.GetTransform())
	{
		Tuple args{ 1 };
		args.SetItem(0, *mEntity.mTransform);

		lst::Logger::log.info("Calling python init\n");
		mModule.CallFunction("init", args);

		// TODO refactor this
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
}


Script::Script(Entity& entity)
:	mEntity{ entity }
,	mModule{ interpreter->ImportModule(entity.GetMesh()->GetName().c_str()) }
,	mArgs  { 2 }
{
	Tuple args{ 1 };
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
