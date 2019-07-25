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


void Script::init( const std::string& script_path )
{
	if ( !interpreter )
	{
		interpreter = std::make_unique<Interpreter>( "sunspot", PyInit_sunspot, pyspot::to_tstring( script_path ).c_str() );
	}
}


Script::Script( const std::string& uri )
    : module{ uri.c_str() }
{
}


void Script::init( gltfspot::Node& n )
{
	auto w_node = Wrapper<gltfspot::Node>( &n );
	module.call( "init", Tuple{ w_node } );
}


void Script::handle( gltfspot::Node& n, const input::Input& input )
{
	if ( auto method = module.get_method( "handle" ) )
	{
		auto w_node  = Wrapper<gltfspot::Node>( &n );
		auto w_input = Wrapper<input::Input>( &(input::Input&)input );
		method->call( Tuple{ w_node, w_input } );
	}
}


void Script::collide( gltfspot::Node& a, gltfspot::Node& b )
{
	if ( auto method = module.get_method( "collide" ) )
	{
		auto w_a = Wrapper<gltfspot::Node>( &a );
		auto w_b = Wrapper<gltfspot::Node>( &b );
		method->call( Tuple{ w_a, w_b } );
	}
}


void Script::update( gltfspot::Node& n, const float delta )
{
	auto w_node = Wrapper<gltfspot::Node>( &n );
	args.SetItem( 0, w_node );
	args.SetItem( 1, delta );
	module.call( "update", args );
}


}  // namespace sunspot
