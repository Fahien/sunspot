#include <memory>
#include <DataSpot.h>

#include "Statement.h"
#include "DataSpotException.h"
#include "sqlite3.h"
#include "WavefrontObject.h"
#include "ModelRepository.h"

#include "Entity.h"
#include "Transform.h"
#include "Mesh.h"

#include "EntityRepository.h"

using namespace sunspot;


EntityRepository::EntityRepository(
	dst::DataSpot& data,
	ModelRepository& modelRepo)
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

	//dst::Statement& stmt{ mData.Prepare("SELECT name FROM main.entity WHERE id = ?;") };
	//stmt.Bind(id);
	//stmt.Step();
	//return stmt.GetText(0);
}



Entity* EntityRepository::loadEntity(const int id)
{
	// Get the entity
	dst::Statement& stmtEntity{ mData.Prepare("SELECT name FROM main.entity WHERE id = ?;") };
	stmtEntity.Bind(id);
	stmtEntity.Step();

	std::string name{ stmtEntity.GetText(0) };

	Entity* pEntity{ new Entity{ id, name } };

	// Get the components
	std::string query{ "SELECT component_id, component_type FROM main.entity_component WHERE entity_id = ?;" };
	dst::Statement& stmtComponents{ mData.Prepare(query) };
	stmtComponents.Bind(id);

	while (true)
	{
		try
		{
			// Step multiple time
			stmtComponents.Step();

			// Get the component id and type
			int         id  { stmtComponents.GetInteger(0) };
			std::string type{ stmtComponents.GetText(1)    };

			// Get the component
			std::string query{ "SELECT * FROM main." + type + " WHERE id = ?;" };
			dst::Statement& stmtComponent{ mData.Prepare(query) };
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
				PySpotVec3 position{ x, y, z };

				// Get rotation.xyz
				x = static_cast<float>(stmtComponent.GetDouble(4));
				y = static_cast<float>(stmtComponent.GetDouble(5));
				z = static_cast<float>(stmtComponent.GetDouble(6));
				PySpotVec3 rotation{ x, y, z };

				// Get scale.xyz
				x = static_cast<float>(stmtComponent.GetDouble(7));
				y = static_cast<float>(stmtComponent.GetDouble(8));
				z = static_cast<float>(stmtComponent.GetDouble(9));
				PySpotVec3 scale{ x, y, z };

				// Construct the component
				PySpotTransform* transform = new PySpotTransform{ id, position, rotation, scale };
				// Add the component to the entity
				pEntity->SetTransform(transform);
			}
			else if (type == "script")
			{
				// Get id
				int id { stmtComponent.GetInteger(0) };

				// Get script name
				std::string name{ stmtComponent.GetText(1) };

				// Construct the component
				Script* script{ new Script{ id, name, *pEntity } };
				pEntity->SetScript(script);
			}
			else if (type == "mesh")
			{
				// Get id
				int id { stmtComponent.GetInteger(0) };

				// Get model path
				std::string path{ stmtComponent.GetText(1) };
				
				// Get mesh name
				std::string name{ stmtComponent.GetText(2) };

				// Get the mesh
				auto mesh = mModelRepository.GetMesh(id, path, name);

				// Construct the component
				pEntity->SetMesh(mesh);
			}
		}
		catch (const dst::DataSpotException&)
		{
			// No more components
			break;
		}
	}

	// If the entity has a Mesh and a Transform
	if (pEntity->GetMesh() && pEntity->GetTransform())
	{
		// Apply the transform to the mesh
		pEntity->GetMesh()->ApplyTransform(*(pEntity->GetTransform()));
	}

	if (pEntity->GetScript())
	{
		// Call the init method in the script
		pEntity->GetScript()->Initialize();
	}

	return pEntity;
}
