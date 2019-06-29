#include <dataspot/Database.h>
#include <dataspot/Exception.h>
#include <dataspot/Statement.h>
#include <hitspot/BoundingBox.h>
#include <sqlite3.h>
#include <memory>

#include "ModelRepository.h"

#include "Mesh.h"
#include "sunspot/component/Camera.h"
#include "sunspot/component/Collider.h"
#include "sunspot/component/Rigidbody.h"
#include "sunspot/component/Transform.h"
#include "sunspot/entity/Entity.h"

#include "EntityRepository.h"

using namespace std;
using namespace sunspot;
using namespace sunspot::component;
using namespace pyspot;
using namespace dataspot;


EntityRepository::EntityRepository( std::string& path, ModelRepository& modelRepo )
    : project_path{ path }, model_repository{ modelRepo }, entities{}
{
}


EntityRepository::~EntityRepository()
{
	for ( auto& pair : entities )
	{
		if ( pair.second )
		{
			delete pair.second;
		}
	}
}


Entity* EntityRepository::load_entity( const int id )
{
	// Check if already loaded
	auto it = entities.find( id );
	if ( it != entities.end() )
	{
		return it->second;
	}

	Entity* pEntity{ _load_entity( id ) };
	entities.emplace( id, pEntity );
	return pEntity;
}


Entity* EntityRepository::_load_entity( const int id )
{
	// Get the entity

	return nullptr;
	/* 
	auto prepare_entity_result = database.prepare( "SELECT name FROM main.entity WHERE id = ?;" );
	if ( auto error = std::get_if<dataspot::Error>( &prepare_entity_result ) )
	{
		return nullptr;
	}

	auto stmt_entity = std::get_if<Statement>( &prepare_entity_result );
	stmt_entity->bind( id );
	stmt_entity->step();

	auto name = stmt_entity->get_text( 0 );

	Entity* pEntity{ new Entity{ id, name } };

	// Get the components
	auto query                     = "SELECT component_id, component_type FROM main.entity_component WHERE entity_id = ?;";
	auto prepare_components_result = database.prepare( query );
	auto stmt_components           = std::get_if<Statement>( &prepare_components_result );

	stmt_components->bind( id );

	while ( true )
	{
		// Step multiple time
		if ( stmt_components->step() != SQLITE_ROW )
		{
			break;
		}

		// Get the component id and type
		int    id{ stmt_components->get_integer( 0 ) };
		string type{ stmt_components->get_text( 1 ) };

		// Get the component
		auto query                    = "SELECT * FROM main." + type + " WHERE id = ?;";
		auto prepare_component_result = database.prepare( query );
		auto stmt_component           = std::get_if<Statement>( &prepare_component_result );

		stmt_component->bind( id );
		stmt_component->step();

		if ( type == "transform" )
		{
			// Get id
			int id{ stmt_component->get_integer( 0 ) };

			// Get position.xyz
			float          x{ static_cast<float>( stmt_component->get_double( 1 ) ) };
			float          y{ static_cast<float>( stmt_component->get_double( 2 ) ) };
			float          z{ static_cast<float>( stmt_component->get_double( 3 ) ) };
			mathspot::Vec3 position{ x, y, z };

			// Get rotation.xyz
			x = static_cast<float>( stmt_component->get_double( 4 ) );
			y = static_cast<float>( stmt_component->get_double( 5 ) );
			z = static_cast<float>( stmt_component->get_double( 6 ) );
			mathspot::Vec3 rotation{ x, y, z };

			// Get scale.xyz
			x = static_cast<float>( stmt_component->get_double( 7 ) );
			y = static_cast<float>( stmt_component->get_double( 8 ) );
			z = static_cast<float>( stmt_component->get_double( 9 ) );
			mathspot::Vec3 scale{ x, y, z };

			// Set the transform values
			auto& transform    = pEntity->GetTransform();
			transform.id       = id;
			transform.position = position;
			transform.rotation = rotation;
			transform.scale    = scale;
		}
		else if ( type == "rigidbody" )
		{
			// Get id
			int id{ stmt_component->get_integer( 0 ) };

			// Get velocity.xyz
			float          x{ static_cast<float>( stmt_component->get_double( 1 ) ) };
			float          y{ static_cast<float>( stmt_component->get_double( 2 ) ) };
			float          z{ static_cast<float>( stmt_component->get_double( 3 ) ) };
			mathspot::Vec3 velocity{ x, y, z };

			// Construct che component
			Rigidbody* rigidbody = new Rigidbody{ id, velocity };
			// Add the component
			pEntity->SetRigidbody( rigidbody );
		}
		else if ( type == "script" )
		{
			// Get id
			int id{ stmt_component->get_integer( 0 ) };

			// Get script name
			string name{ stmt_component->get_text( 1 ) };

			// Construct the component
			Script* script{ new Script{ id, name, *pEntity } };
			pEntity->SetScript( script );
		}
		else if ( type == "mesh" )
		{
			// Get id
			int id{ stmt_component->get_integer( 0 ) };

			// Get model path
			string path{ stmt_component->get_text( 1 ) };

			// Get mesh name
			string name{ stmt_component->get_text( 2 ) };

			// Get the mesh
			auto& model = model_repository.get_model( id, path, name );

			// Construct the component
			pEntity->add<component::Model>( model );
		}
		else if ( type == "collider" )
		{
			// Get id
			int id{ stmt_component->get_integer( 0 ) };

			// Get name
			string name{ stmt_component->get_text( 1 ) };

			// TODO Bounding could be of different types
			// Implement circles, etc..

			// Get box width and height
			hitspot::BoundingBox box{ pEntity };
			box.width  = static_cast<float>( stmt_component->get_double( 2 ) );
			box.height = static_cast<float>( stmt_component->get_double( 3 ) );

			auto collider = new Collider{ id, name, *pEntity, move( box ) };
			pEntity->add( *collider );
		}
		else if ( type == "camera" )
		{
			int    id{ stmt_component->get_integer( 0 ) };
			string name{ stmt_component->get_text( 1 ) };

			auto width = 50.0f;
			auto r     = width / 2.0f;
			auto l     = -r;
			auto t     = r * 540.0f / 960.0f;
			auto b     = -t;

			auto pCamera = new component::OrthographicCamera( r, l, t, b );
			pCamera->set_id( id );
			pCamera->set_name( name );
			pCamera->set_parent( *pEntity );

			pEntity->add<component::Camera>( *pCamera );
		}
	}

	// If the entity has a Mesh and a Transform
	if ( auto model = pEntity->get<component::Model>() )
	{
		// Apply the transform to the mesh
		auto& node      = model->get().GetNode();
		auto& transform = pEntity->GetTransform();
		node.matrix.ScaleX( transform.scale.x );
		node.matrix.ScaleY( transform.scale.y );
		node.matrix.ScaleZ( transform.scale.z );
		node.matrix.TranslateX( transform.position.x );
		node.matrix.TranslateY( transform.position.y );
		node.matrix.TranslateZ( transform.position.z );
		node.matrix.RotateX( transform.rotation.x );
		node.matrix.RotateY( transform.rotation.y );
		node.matrix.RotateZ( transform.rotation.z );
	}

	if ( pEntity->GetScript() )
	{
		// Call the init method in the script
		pEntity->GetScript()->initialize();
	}

	return pEntity;
	*/
}
