#ifndef SST_ENTITYREPOSITORY_H_
#define SST_ENTITYREPOSITORY_H_

#include <string>
#include <map>

#include "view/GltfCamera.h"

namespace dataspot
{
	class DataSpot;
}

namespace dst = dataspot;

namespace sunspot
{

class Entity;
class ModelRepository;

class EntityRepository
{
  public:
	EntityRepository(dst::DataSpot& data, ModelRepository& modelRepo);
	~EntityRepository();

	Entity* LoadEntity(const int id);

  private:
	Entity* loadEntity(const int id);

	dst::DataSpot& m_Data;
	ModelRepository& m_ModelRepository;

	std::map<const int, Entity*> m_Entities;
};


}


#endif // SST_ENTITYREPOSITORY_H_
