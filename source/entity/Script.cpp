#include <Logger.h>
#include <pyspot/Tuple.h>
#include <pyspot/String.h>
#include <pyspot/Dictionary.h>
#include <sunspot/extension/Sunspot.h>
#include <sunspot/component/Transform.h>
#include <sunspot/component/Rigidbody.h>

#include "Entity.h"
#include "Mesh.h"

#include "Script.h"

using namespace sunspot;
using namespace pyspot;
namespace mst = mathspot;
namespace lst = logspot;


std::unique_ptr<Interpreter> Script::interpreter{ nullptr };


void Script::Initialize(const std::wstring& scriptPath)
{
	if (!interpreter)
	{
		interpreter = std::make_unique<Interpreter>("sunspot", PyInit_Sunspot, scriptPath.c_str());
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
		Dictionary dict{};
		dict.SetItem("transform", *mEntity.mTransform);
		dict.SetItem("rigidbody", *mEntity.mRigidbody);
		Tuple args{ dict };

		lst::Logger::log.info("Calling python init\n");
		mModule.CallFunction("init", args);

		// TODO refactor this
		mEntity.mModel->GetNode().matrix.ScaleX(mEntity.mTransform->GetScale().GetX());
		mEntity.mModel->GetNode().matrix.ScaleY(mEntity.mTransform->GetScale().GetY());
		mEntity.mModel->GetNode().matrix.ScaleZ(mEntity.mTransform->GetScale().GetZ());
		mEntity.mModel->GetNode().matrix.TranslateX(mEntity.mTransform->GetPosition().GetX());
		mEntity.mModel->GetNode().matrix.TranslateY(mEntity.mTransform->GetPosition().GetY());
		mEntity.mModel->GetNode().matrix.TranslateZ(mEntity.mTransform->GetPosition().GetZ());
		mEntity.mModel->GetNode().matrix.RotateX(mEntity.mTransform->GetRotation().GetX());
		mEntity.mModel->GetNode().matrix.RotateY(mEntity.mTransform->GetRotation().GetY());
		mEntity.mModel->GetNode().matrix.RotateZ(mEntity.mTransform->GetRotation().GetZ());
	}
}


Script::Script(Entity& entity)
:	mEntity{ entity }
,	mModule{ interpreter->ImportModule(entity.GetModel()->GetNode().name.c_str()) }
,	mArgs  { 3 }
{
	Dictionary dict{};
	dict.SetItem("transform", *mEntity.mTransform);
	dict.SetItem("rigidbody", *mEntity.mRigidbody);
	Tuple args{ dict };

	lst::Logger::log.info("Calling python init\n");
	mModule.CallFunction("init", args);

	mEntity.mModel->GetNode().matrix.ScaleX(mEntity.mTransform->GetScale().GetX());
	mEntity.mModel->GetNode().matrix.ScaleY(mEntity.mTransform->GetScale().GetY());
	mEntity.mModel->GetNode().matrix.ScaleZ(mEntity.mTransform->GetScale().GetZ());
	mEntity.mModel->GetNode().matrix.TranslateX(mEntity.mTransform->GetPosition().GetX());
	mEntity.mModel->GetNode().matrix.TranslateY(mEntity.mTransform->GetPosition().GetY());
	mEntity.mModel->GetNode().matrix.TranslateZ(mEntity.mTransform->GetPosition().GetZ());
	mEntity.mModel->GetNode().matrix.RotateX(mEntity.mTransform->GetRotation().GetX());
	mEntity.mModel->GetNode().matrix.RotateY(mEntity.mTransform->GetRotation().GetY());
	mEntity.mModel->GetNode().matrix.RotateZ(mEntity.mTransform->GetRotation().GetZ());
}


void Script::Handle(const input::Input& input)
{
	Tuple args{ input };
	mModule.CallFunction("handle", args);
}


void Script::Update(const float delta, const input::Input& input)
{
	mArgs.SetItem(0, delta);
	mArgs.SetItem(1, input);
	
	mModule.CallFunction("update", mArgs);
	
	mEntity.mModel->GetNode().matrix = mst::Mat4::identity;
	mEntity.mModel->GetNode().matrix.ScaleX(mEntity.mTransform->GetScale().GetX());
	mEntity.mModel->GetNode().matrix.ScaleY(mEntity.mTransform->GetScale().GetY());
	mEntity.mModel->GetNode().matrix.ScaleZ(mEntity.mTransform->GetScale().GetZ());
	mEntity.mModel->GetNode().matrix.TranslateX(mEntity.mTransform->GetPosition().GetX());
	mEntity.mModel->GetNode().matrix.TranslateY(mEntity.mTransform->GetPosition().GetY());
	mEntity.mModel->GetNode().matrix.TranslateZ(mEntity.mTransform->GetPosition().GetZ());
	mEntity.mModel->GetNode().matrix.RotateX(mEntity.mTransform->GetRotation().GetX());
	mEntity.mModel->GetNode().matrix.RotateY(mEntity.mTransform->GetRotation().GetY());
	mEntity.mModel->GetNode().matrix.RotateZ(mEntity.mTransform->GetRotation().GetZ());
}
