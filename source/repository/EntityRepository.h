#ifndef SST_ENTITYREPOSITORY_H_
#define SST_ENTITYREPOSITORY_H_

#include <map>
#include <string>

#include "view/GltfCamera.h"

namespace dataspot
{
class Database;
}

namespace dst = dataspot;

namespace sunspot
{
class Entity;
class ModelRepository;

class EntityRepository
{
  public:
	EntityRepository( dst::Database& data, ModelRepository& model_repo );
	~EntityRepository();

	Entity* load_entity( const int id );

  private:
	Entity* _load_entity( const int id );

	dst::Database&   database;
	ModelRepository& model_repository;

	std::map<const int, Entity*> entities;
};


}  // namespace sunspot


#endif  // SST_ENTITYREPOSITORY_H_
