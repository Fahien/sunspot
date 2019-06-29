#include <logspot/Log.h>

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


void Script::initialize( const std::string& script_path )
{
	if ( !interpreter )
	{
		interpreter = std::make_unique<Interpreter>( "sunspot", PyInit_sunspot, pyspot::to_tstring( script_path ).c_str() );
	}
}


Script::Script( const int id, std::string& name, Entity& entity )
    : Object{ id, name }
    , entity{ entity }
    , module{ name.c_str() }
{
}


void Script::initialize()
{
	Module     utils{ "utils" };
	Dictionary dict{ utils.Invoke( "Map" ) };
	auto       wRigidbody = Wrapper<component::Rigidbody>( entity.mRigidbody );

	if ( auto transform = entity.get<component::Transform>() )
	{
		auto wTransform = Wrapper<component::Transform>( transform );
		dict.SetItem( "transform", wTransform );
		dict.SetItem( "rigidbody", wRigidbody );

		lst::Log::info( "%s.init", module.GetName().c_str() );
		module.Invoke( "init", Tuple{ dict } );

		// TODO refactor this
		if ( auto model = entity.get<component::Model>() )
		{
			model->GetNode().matrix.ScaleX( transform->scale.x );
			model->GetNode().matrix.ScaleY( transform->scale.y );
			model->GetNode().matrix.ScaleZ( transform->scale.z );
			model->GetNode().matrix.TranslateX( transform->position.x );
			model->GetNode().matrix.TranslateY( transform->position.y );
			model->GetNode().matrix.TranslateZ( transform->position.z );
			model->GetNode().matrix.RotateX( transform->rotation.x );
			model->GetNode().matrix.RotateY( transform->rotation.y );
			model->GetNode().matrix.RotateZ( transform->rotation.z );
		}
	}
}


Script::Script( Entity& entity )
    : entity{ entity }
    , module{ entity.get<component::Model>()->GetNode().name.c_str() }
{
	Dictionary dict{};
	if ( auto transform = entity.get<component::Transform>() )
	{
		auto wTransform = Wrapper<component::Transform>( transform );
		auto wRigidbody = Wrapper<component::Rigidbody>( entity.mRigidbody );
		dict.SetItem( "transform", wTransform );
		dict.SetItem( "rigidbody", wRigidbody );
		Tuple args{ dict };

		lst::Log::info( "%s.init", module.GetName().c_str() );
		module.Invoke( "init", args );

		if ( auto model = entity.get<component::Model>() )
		{
			model->GetNode().matrix.ScaleX( transform->scale.x );
			model->GetNode().matrix.ScaleY( transform->scale.y );
			model->GetNode().matrix.ScaleZ( transform->scale.z );
			model->GetNode().matrix.TranslateX( transform->position.x );
			model->GetNode().matrix.TranslateY( transform->position.y );
			model->GetNode().matrix.TranslateZ( transform->position.z );
			model->GetNode().matrix.RotateX( transform->rotation.x );
			model->GetNode().matrix.RotateY( transform->rotation.y );
			model->GetNode().matrix.RotateZ( transform->rotation.z );
		}
	}
}


void Script::handle( const input::Input& input )
{
	Tuple args{ Wrapper<input::Input>( &(input::Input&)input ) };
	module.Invoke( "handle", args );
}


void Script::collide( Entity& other )
{
	Module     utils{ "utils" };
	Dictionary dict{ utils.Invoke( "Map" ) };
	auto       wRigidbody = Wrapper<component::Rigidbody>( other.mRigidbody );
	dict.SetItem( "rigidbody", wRigidbody );
	Tuple args{ dict };

	module.Invoke( "collide", args );
}


void Script::update( const float delta )
{
	args.SetItem( 0, delta );
	module.Invoke( "update", args );

	if ( auto model = entity.get<component::Model>() )
	{
		model->GetNode().matrix = mst::Mat4::identity;

		if ( auto transform = entity.get<component::Transform>() )
		{
			model->GetNode().matrix.ScaleX( transform->scale.x );
			model->GetNode().matrix.ScaleY( transform->scale.y );
			model->GetNode().matrix.ScaleZ( transform->scale.z );
			model->GetNode().matrix.TranslateX( transform->position.x );
			model->GetNode().matrix.TranslateY( transform->position.y );
			model->GetNode().matrix.TranslateZ( transform->position.z );
			model->GetNode().matrix.RotateX( transform->rotation.x );
			model->GetNode().matrix.RotateY( transform->rotation.y );
			model->GetNode().matrix.RotateZ( transform->rotation.z );
		}
	}
}


}  // namespace sunspot
