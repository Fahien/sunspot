#ifndef SST_ENTITYREPOSITORY_H_
#define SST_ENTITYREPOSITORY_H_

#include <map>
#include <string>

#include "view/GltfCamera.h"

namespace dataspot
{
class Dataspot;
}

namespace dst = dataspot;

namespace sunspot
{
class Entity;
class ModelRepository;

class EntityRepository
{
  public:
	EntityRepository( dst::Dataspot& data, ModelRepository& modelRepo );
	~EntityRepository();

	Entity* LoadEntity( const int id );

  private:
	Entity* loadEntity( const int id );

	dst::Dataspot&   m_Data;
	ModelRepository& m_ModelRepository;

	std::map<const int, Entity*> m_Entities;
};


}  // namespace sunspot


#endif  // SST_ENTITYREPOSITORY_H_
