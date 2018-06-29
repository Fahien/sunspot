#include <memory>
#include <dataspot/DataSpot.h>
#include <hitspot/BoundingBox.h>
#include <dataspot/Statement.h>
#include <dataspot/Exception.h>
#include <sqlite3.h>

#include "WavefrontObject.h"
#include "ModelRepository.h"

#include "sunspot/entity/Entity.h"
#include "sunspot/component/Collider.h"
#include "sunspot/component/Transform.h"
#include "sunspot/component/Rigidbody.h"
#include "Mesh.h"

#include "EntityRepository.h"

using namespace std;
using namespace sunspot;
using namespace sunspot::component;
using namespace pyspot;
using namespace dataspot;


EntityRepository::EntityRepository(DataSpot& data, ModelRepository& modelRepo)
:	mData{ data }
,	mModelRepository{ modelRepo }
,	mEntities{}
{}


EntityRepository::~EntityRepository()
{
	for (auto& pair : mEntities)
	{
		if (pair.second)
		{
			delete pair.second;
		}
	}
}


Entity* EntityRepository::LoadEntity(const int id)
{
	auto it = mEntities.find(id);
	if (it != mEntities.end())
	{
		return it->second;
	}

	Entity* pEntity{ loadEntity(id) };
	mEntities.emplace(id, pEntity);
	return pEntity;

	//Statement& stmt{ mData.Prepare("SELECT name FROM main.entity WHERE id = ?;") };
	//stmt.Bind(id);
	//stmt.Step();
	//return stmt.GetText(0);
}



Entity* EntityRepository::loadEntity(const int id)
{
	// Get the entity
	Statement& stmtEntity{ mData.Prepare("SELECT name FROM main.entity WHERE id = ?;") };
	stmtEntity.Bind(id);
	stmtEntity.Step();

	string name{ stmtEntity.GetText(0) };

	Entity* pEntity{ new Entity{ id, name } };

	// Get the components
	string query{ "SELECT component_id, component_type FROM main.entity_component WHERE entity_id = ?;" };
	Statement& stmtComponents{ mData.Prepare(query) };
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
			Statement& stmtComponent{ mData.Prepare(query) };
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
				Vec3 position{ x, y, z };

				// Get rotation.xyz
				x = static_cast<float>(stmtComponent.GetDouble(4));
				y = static_cast<float>(stmtComponent.GetDouble(5));
				z = static_cast<float>(stmtComponent.GetDouble(6));
				Vec3 rotation{ x, y, z };

				// Get scale.xyz
				x = static_cast<float>(stmtComponent.GetDouble(7));
				y = static_cast<float>(stmtComponent.GetDouble(8));
				z = static_cast<float>(stmtComponent.GetDouble(9));
				Vec3 scale{ x, y, z };

				// Construct the component
				Transform* transform = new Transform{ id, position, rotation, scale };
				// Add the component to the entity
				pEntity->SetTransform(transform);
			}
			else if (type == "rigidbody")
			{
				// Get id
				int id { stmtComponent.GetInteger(0) };

				// Get velocity.xyz
				float x{ static_cast<float>(stmtComponent.GetDouble(1)) };
				float y{ static_cast<float>(stmtComponent.GetDouble(2)) };
				float z{ static_cast<float>(stmtComponent.GetDouble(3)) };
				Vec3 velocity{ x, y, z };

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
				auto& model = mModelRepository.GetModel(id, path, name);

				// Construct the component
				pEntity->SetModel(&model);
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
		}

	// If the entity has a Mesh and a Transform
	if (pEntity->GetModel() && pEntity->GetTransform())
	{
		// Apply the transform to the mesh
		auto& node      = pEntity->GetModel()->GetNode();
		auto pTransform = pEntity->GetTransform();
		node.matrix.ScaleX(pTransform->GetScale().GetX());
		node.matrix.ScaleY(pTransform->GetScale().GetY());
		node.matrix.ScaleZ(pTransform->GetScale().GetZ());
		node.matrix.TranslateX(pTransform->GetPosition().GetX());
		node.matrix.TranslateY(pTransform->GetPosition().GetY());
		node.matrix.TranslateZ(pTransform->GetPosition().GetZ());
		node.matrix.RotateX(pTransform->GetRotation().GetX());
		node.matrix.RotateY(pTransform->GetRotation().GetY());
		node.matrix.RotateZ(pTransform->GetRotation().GetZ());
	}

	if (pEntity->GetScript())
	{
		// Call the init method in the script
		pEntity->GetScript()->Initialize();
	}

	return pEntity;
}
