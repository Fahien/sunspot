#ifndef SST_ENTITYREPOSITORY_H_
#define SST_ENTITYREPOSITORY_H_

#include <string>
#include <map>


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

	dst::DataSpot& mData;
	ModelRepository& mModelRepository;

	std::map<const int, Entity*> mEntities;
};


}


#endif // SST_ENTITYREPOSITORY_H_
