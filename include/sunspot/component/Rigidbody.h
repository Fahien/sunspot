#ifndef SST_RIGIDBODY_H_
#define SST_RIGIDBODY_H_

#include <mathspot/Math.h>

#include "sunspot/util/Export.h"


namespace sunspot
{

namespace component
{


class PYSPOT_EXPORT Rigidbody
{
  public:
	int id;
	mathspot::Vec3 velocity;
};


} // namespace component

} // namespace sunspot


#endif // SST_RIGIDBODY_H_
