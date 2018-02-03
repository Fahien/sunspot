#ifndef SST_ENTITYDATA_H_
#define SST_ENTITYDATA_H_

#include "Entity.h"

#include <DataSpot.h>

#include <string>

namespace dst = dataspot;


namespace sunspot
{


class EntityData
{
public:
	EntityData(dst::DataSpot& data);
	~EntityData();

	std::string LoadEntity(const int id);

private:
	Entity loadEntity(const int id);

	dst::DataSpot& mData;
};


}


#endif // SST_ENTITYDATA_H_
