#ifndef SST_RIGIDBODY_H_
#define SST_RIGIDBODY_H_

#include <mathspot/Math.h>


namespace sunspot
{

namespace component
{


class [[pyspot::export]] Rigidbody
{
  public:
	int id;
	mathspot::Vec3 velocity;
};


} // namespace component

} // namespace sunspot


#endif // SST_RIGIDBODY_H_
