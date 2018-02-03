#include "EntityData.h"

#include "Statement.h"
#include "DataSpotException.h"
#include "sqlite3.h"

#include "Transform.h"


namespace sst = sunspot;


sst::EntityData::EntityData(dst::DataSpot& data)
:	mData{ data }
{}


sst::EntityData::~EntityData()
{}


std::string sst::EntityData::LoadEntity(const int id)
{	
	// sst::Entity entity{ loadEntity(id) };
	dst::Statement& stmt{ mData.Prepare("SELECT name FROM main.entity WHERE id = ?;") };
	stmt.Bind(id);
	stmt.Step();
	return stmt.GetText(0);
}


sst::Entity sst::EntityData::loadEntity(const int id)
{
	dst::Statement& stmtEntity{ mData.Prepare("SELECT name FROM main.entity WHERE id = ?;") };
	stmtEntity.Bind(id);
	stmtEntity.Step();

	std::string name{ stmtEntity.GetText(0) };

	sst::Entity entity{ id, name };

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
				PySpotVec3 position{
					static_cast<float>(stmtComponent.GetDouble(1)),
					static_cast<float>(stmtComponent.GetDouble(2)),
					static_cast<float>(stmtComponent.GetDouble(3))
				};

				// Get scale.xyz
				PySpotVec3 scale{
					static_cast<float>(stmtComponent.GetDouble(4)),
					static_cast<float>(stmtComponent.GetDouble(5)),
					static_cast<float>(stmtComponent.GetDouble(6))
				};

				// Get rotation.xyz
				PySpotVec3 rotation{
					static_cast<float>(stmtComponent.GetDouble(7)),
					static_cast<float>(stmtComponent.GetDouble(8)),
					static_cast<float>(stmtComponent.GetDouble(9))
				};

				// Construct the component
				PySpotTransform* transform = new PySpotTransform{ id, position, scale, rotation };
				// Add the component to the entity
				entity.SetTransform(transform);
			}

		}
		catch (const dst::DataSpotException&)
		{
			// No more components
			break;
		}
	}


	return entity;
}
