#include <memory>
#include <dataspot/DataSpot.h>
#include <hitspot/BoundingBox.h>
#include <dataspot/Statement.h>
#include <dataspot/Exception.h>
#include <sqlite3.h>

#include "ModelRepository.h"

#include "sunspot/entity/Entity.h"
#include "sunspot/component/Collider.h"
#include "sunspot/component/Transform.h"
#include "sunspot/component/Rigidbody.h"
#include "sunspot/component/Camera.h"
#include "Mesh.h"

#include "EntityRepository.h"

using namespace std;
using namespace sunspot;
using namespace sunspot::component;
using namespace pyspot;
using namespace dataspot;


EntityRepository::EntityRepository( DataSpot& data, ModelRepository& modelRepo )
:	m_Data { data }
,	m_ModelRepository { modelRepo }
,	m_Entities {}
{}


EntityRepository::~EntityRepository()
{
	for ( auto& pair : m_Entities )
	{
		if ( pair.second )
		{
			delete pair.second;
		}
	}
}


Entity* EntityRepository::LoadEntity( const int id )
{
	// Check if already loaded
	auto it = m_Entities.find( id );
	if (it != m_Entities.end())
	{
		return it->second;
	}

	Entity* pEntity{ loadEntity( id ) };
	m_Entities.emplace( id, pEntity );
	return pEntity;
}



Entity* EntityRepository::loadEntity(const int id)
{
	// Get the entity
	Statement& stmtEntity{ m_Data.Prepare("SELECT name FROM main.entity WHERE id = ?;") };
	stmtEntity.Bind(id);
	stmtEntity.Step();

	string name{ stmtEntity.GetText(0) };

	Entity* pEntity{ new Entity{ id, name } };

	// Get the components
	string query{ "SELECT component_id, component_type FROM main.entity_component WHERE entity_id = ?;" };
	Statement& stmtComponents{ m_Data.Prepare(query) };
	stmtComponents.Bind(id);

	while (true)
	{
			// Step multiple time
			if (stmtComponents.Step() != SQLITE_ROW)
			{
				break;
			}

			// Get the component id and type
			int    id  { stmtComponents.GetInteger(0) };
			string type{ stmtComponents.GetText(1)    };

			// Get the component
			string query{ "SELECT * FROM main." + type + " WHERE id = ?;" };
			Statement& stmtComponent{ m_Data.Prepare(query) };
			stmtComponent.Bind(id);
			stmtComponent.Step();

			if (type == "transform")
			{
				// Get id
				int id { stmtComponent.GetInteger(0) };

				// Get position.xyz
				float x{ static_cast<float>(stmtComponent.GetDouble(1)) };
				float y{ static_cast<float>(stmtComponent.GetDouble(2)) };
				float z{ static_cast<float>(stmtComponent.GetDouble(3)) };
				mathspot::Vec3 position{ x, y, z };

				// Get rotation.xyz
				x = static_cast<float>(stmtComponent.GetDouble(4));
				y = static_cast<float>(stmtComponent.GetDouble(5));
				z = static_cast<float>(stmtComponent.GetDouble(6));
				mathspot::Vec3 rotation{ x, y, z };

				// Get scale.xyz
				x = static_cast<float>(stmtComponent.GetDouble(7));
				y = static_cast<float>(stmtComponent.GetDouble(8));
				z = static_cast<float>(stmtComponent.GetDouble(9));
				mathspot::Vec3 scale{ x, y, z };

				// Set the transform values
				auto& transform = pEntity->GetTransform();
				transform.id = id;
				transform.position = position;
				transform.rotation = rotation;
				transform.scale = scale;
			}
			else if (type == "rigidbody")
			{
				// Get id
				int id { stmtComponent.GetInteger(0) };

				// Get velocity.xyz
				float x{ static_cast<float>(stmtComponent.GetDouble(1)) };
				float y{ static_cast<float>(stmtComponent.GetDouble(2)) };
				float z{ static_cast<float>(stmtComponent.GetDouble(3)) };
				mathspot::Vec3 velocity{ x, y, z };

				// Construct che component
				Rigidbody* rigidbody = new Rigidbody{ id, velocity };
				// Add the component
				pEntity->SetRigidbody(rigidbody);
			}
			else if (type == "script")
			{
				// Get id
				int id { stmtComponent.GetInteger(0) };

				// Get script name
				string name{ stmtComponent.GetText(1) };

				// Construct the component
				Script* script{ new Script{ id, name, *pEntity } };
				pEntity->SetScript(script);
			}
			else if (type == "mesh")
			{
				// Get id
				int id { stmtComponent.GetInteger(0) };

				// Get model path
				string path{ stmtComponent.GetText(1) };

				// Get mesh name
				string name{ stmtComponent.GetText(2) };

				// Get the mesh
				auto& model = m_ModelRepository.get_model(id, path, name);

				// Construct the component
				pEntity->Add<component::Model>( model );
			}
			else if (type == "collider")
			{
				// Get id
				int id { stmtComponent.GetInteger(0) };

				// Get name
				string name{ stmtComponent.GetText(1) };

				// TODO Bounding could be of different types
				// Implement circles, etc..

				// Get box width and height
				hitspot::BoundingBox box{ pEntity };
				box.width  = static_cast<float>(stmtComponent.GetDouble(2));
				box.height = static_cast<float>(stmtComponent.GetDouble(3));

				Collider* collider{ new Collider{ id, name, *pEntity, move(box) } };
				pEntity->SetCollider(collider);
			}
			else if ( type == "camera" )
			{
				int id { stmtComponent.GetInteger( 0 ) };
				string name { stmtComponent.GetText( 1 ) };

				auto width = 50.0f;
				auto r = width / 2.0f;
				auto l = -r;
				auto t = r * 540.0f / 960.0f;
				auto b = -t;

				auto pCamera = new component::OrthographicCamera(r, l, t, b);
				pCamera->SetId( id );
				pCamera->SetName( name );
				pCamera->SetParent( pEntity );

				pEntity->Add<component::Camera>( *pCamera );
			}
		}

	// If the entity has a Mesh and a Transform
	if ( auto model = pEntity->Get<component::Model>() )
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

	if (pEntity->GetScript())
	{
		// Call the init method in the script
		pEntity->GetScript()->Initialize();
	}

	return pEntity;
}
