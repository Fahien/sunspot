#ifndef SST_COLLIDER_H_
#define SST_COLLIDER_H_

#include <string>
#include <hitspot/BoundingBox.h>
#include <sunspot/entity/Object.h>

namespace hst = hitspot;

namespace sunspot
{

class Entity;

namespace component
{


class Collider : public Object
{
  public:
	Collider(const int id, std::string& name, Entity& e, hst::BoundingBox&& b)
	: Object{ id, name }, mEntity{ e }, mBox{ b } {}

	Entity& GetEntity() { return mEntity; }
	hst::BoundingBox& GetBoundingBox() { return mBox; }

  private:
	Entity& mEntity;
	hst::BoundingBox mBox;
};


}

}


#endif // SST_COLLIDER_H_
