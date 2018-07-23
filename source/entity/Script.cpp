#include <logspot/Logger.h>

#include "Mesh.h"
#include "sunspot/entity/Entity.h"
#include "sunspot/component/Model.h"
// Do not reorder, there is a conflict between json.h and Python.h
#include <pyspot/Tuple.h>
#include <pyspot/String.h>
#include <pyspot/Dictionary.h>
#include <pyspot/Error.h>

#include "sunspot/extension/Sunspot.h"
#include "sunspot/component/Transform.h"
#include "sunspot/component/Rigidbody.h"

#include "sunspot/entity/Script.h"

using namespace sunspot;
using namespace pyspot;
namespace mst = mathspot;
namespace lst = logspot;


std::unique_ptr<Interpreter> Script::interpreter{ nullptr };


void Script::Initialize(const tstring& scriptPath)
{
	if (!interpreter)
	{
		interpreter = std::make_unique<Interpreter>("sunspot", PyInit_Sunspot, scriptPath.c_str());
	}
}


Script::Script(const int id, std::string& name, Entity& entity)
:	Object { id, name }
,	mEntity{ entity }
,	mModule{ name.c_str() }
{}


void Script::Initialize()
{
	if (mEntity.GetTransform())
	{
		Module utils{ "utils" };
		Dictionary dict{ utils.Invoke("Map") };
		dict.SetItem("transform", *mEntity.mTransform);
		dict.SetItem("rigidbody", *mEntity.mRigidbody);

		lst::Logger::log.Info("%s.init", mModule.GetName().c_str());
		mModule.Invoke("init", Tuple{ dict });

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
,	mModule{ entity.GetModel()->GetNode().name.c_str() }
{
	Dictionary dict{};
	dict.SetItem("transform", *mEntity.mTransform);
	dict.SetItem("rigidbody", *mEntity.mRigidbody);
	Tuple args{ dict };

	lst::Logger::log.Info("%s.init", mModule.GetName().c_str());
	mModule.Invoke("init", args);

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
	mModule.Invoke("handle", args);
}


void Script::Collide(Entity& other)
{
	Module utils{ "utils" };
	Dictionary dict{ utils.Invoke("Map") };
	dict.SetItem("rigidbody", *other.mRigidbody);
	Tuple args{ dict };

	mModule.Invoke("collide", args);
}


void Script::Update(const float delta)
{
	mArgs.SetItem(0, delta);
	mModule.Invoke("update", mArgs);
	
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
