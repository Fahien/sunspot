#ifndef SST_RIGIDBODY_H_
#define SST_RIGIDBODY_H_

#include <mathspot/Math.h>
#include <sunspot/entity/Object.h>

#include "sunspot/util/Export.h"

namespace sunspot::component
{
class PYSPOT_EXPORT Rigidbody : public Object
{
  public:
	int            id;
	mathspot::Vec3 velocity;
};


}  // namespace sunspot::component

#endif  // SST_RIGIDBODY_H_
