#include <logspot/Logger.h>

#include "Mesh.h"
#include "sunspot/component/Model.h"
#include "sunspot/entity/Entity.h"
// Do not reorder, there is a conflict between json.h and Python.h
#include <pyspot/Dictionary.h>
#include <pyspot/Error.h>
#include <pyspot/String.h>
#include <pyspot/Tuple.h>

#include "pyspot/Extension.h"
#include "pyspot/Wrapper.h"
#include "sunspot/component/Rigidbody.h"
#include "sunspot/component/Transform.h"

#include "sunspot/entity/Script.h"

using namespace pyspot;
namespace mst = mathspot;
namespace lst = logspot;


namespace sunspot
{
std::unique_ptr<Interpreter> Script::interpreter{ nullptr };


void Script::Initialize( const std::string& scriptPath )
{
	if ( !interpreter )
	{
		interpreter = std::make_unique<Interpreter>( "sunspot", PyInit_sunspot, pyspot::to_tstring( scriptPath ).c_str() );
	}
}


Script::Script( const int id, std::string& name, Entity& entity )
    : Object{ id, name }, mEntity{ entity }, mModule{ name.c_str() }
{
}


void Script::Initialize()
{
	Module     utils{ "utils" };
	Dictionary dict{ utils.Invoke( "Map" ) };
	auto       wRigidbody = Wrapper<component::Rigidbody>( mEntity.mRigidbody );
	auto       wTransform = Wrapper<component::Transform>( &mEntity.mTransform );
	dict.SetItem( "transform", wTransform );
	dict.SetItem( "rigidbody", wRigidbody );

	lst::Logger::log.Info( "%s.init", mModule.GetName().c_str() );
	mModule.Invoke( "init", Tuple{ dict } );

	// TODO refactor this
	auto& model = mEntity.Get<component::Model>()->get();
	model.GetNode().matrix.ScaleX( mEntity.mTransform.scale.x );
	model.GetNode().matrix.ScaleY( mEntity.mTransform.scale.y );
	model.GetNode().matrix.ScaleZ( mEntity.mTransform.scale.z );
	model.GetNode().matrix.TranslateX( mEntity.mTransform.position.x );
	model.GetNode().matrix.TranslateY( mEntity.mTransform.position.y );
	model.GetNode().matrix.TranslateZ( mEntity.mTransform.position.z );
	model.GetNode().matrix.RotateX( mEntity.mTransform.rotation.x );
	model.GetNode().matrix.RotateY( mEntity.mTransform.rotation.y );
	model.GetNode().matrix.RotateZ( mEntity.mTransform.rotation.z );
}


Script::Script( Entity& entity ) : mEntity{ entity }, mModule{ entity.Get<component::Model>()->get().GetNode().name.c_str() }
{
	Dictionary dict{};
	auto       wTransform = Wrapper<component::Transform>( &mEntity.mTransform );
	auto       wRigidbody = Wrapper<component::Rigidbody>( mEntity.mRigidbody );
	dict.SetItem( "transform", wTransform );
	dict.SetItem( "rigidbody", wRigidbody );
	Tuple args{ dict };

	lst::Logger::log.Info( "%s.init", mModule.GetName().c_str() );
	mModule.Invoke( "init", args );

	auto& model = mEntity.Get<component::Model>()->get();
	model.GetNode().matrix.ScaleX( mEntity.mTransform.scale.x );
	model.GetNode().matrix.ScaleY( mEntity.mTransform.scale.y );
	model.GetNode().matrix.ScaleZ( mEntity.mTransform.scale.z );
	model.GetNode().matrix.TranslateX( mEntity.mTransform.position.x );
	model.GetNode().matrix.TranslateY( mEntity.mTransform.position.y );
	model.GetNode().matrix.TranslateZ( mEntity.mTransform.position.z );
	model.GetNode().matrix.RotateX( mEntity.mTransform.rotation.x );
	model.GetNode().matrix.RotateY( mEntity.mTransform.rotation.y );
	model.GetNode().matrix.RotateZ( mEntity.mTransform.rotation.z );
}


void Script::Handle( const input::Input& input )
{
	Tuple args{ Wrapper<input::Input>( &(input::Input&)input ) };
	mModule.Invoke( "handle", args );
}


void Script::Collide( Entity& other )
{
	Module     utils{ "utils" };
	Dictionary dict{ utils.Invoke( "Map" ) };
	auto       wRigidbody = Wrapper<component::Rigidbody>( other.mRigidbody );
	dict.SetItem( "rigidbody", wRigidbody );
	Tuple args{ dict };

	mModule.Invoke( "collide", args );
}


void Script::Update( const float delta )
{
	mArgs.SetItem( 0, delta );
	mModule.Invoke( "update", mArgs );

	auto& model            = mEntity.Get<component::Model>()->get();
	model.GetNode().matrix = mst::Mat4::identity;
	model.GetNode().matrix.ScaleX( mEntity.mTransform.scale.x );
	model.GetNode().matrix.ScaleY( mEntity.mTransform.scale.y );
	model.GetNode().matrix.ScaleZ( mEntity.mTransform.scale.z );
	model.GetNode().matrix.TranslateX( mEntity.mTransform.position.x );
	model.GetNode().matrix.TranslateY( mEntity.mTransform.position.y );
	model.GetNode().matrix.TranslateZ( mEntity.mTransform.position.z );
	model.GetNode().matrix.RotateX( mEntity.mTransform.rotation.x );
	model.GetNode().matrix.RotateY( mEntity.mTransform.rotation.y );
	model.GetNode().matrix.RotateZ( mEntity.mTransform.rotation.z );
}


}  // namespace sunspot
