#ifndef SST_TRANSFORM_H_
#define SST_TRANSFORM_H_

#include <mathspot/Math.h>

#include "sunspot/util/Export.h"

namespace sunspot::component
{


class PYSPOT_EXPORT Transform
{
  public:
	int id;
	mathspot::Vec3 position;
	mathspot::Vec3 rotation;
	mathspot::Vec3 scale { 1.0f, 1.0f, 1.0f };
};


} // namespace sunspot::component


#endif // SST_TRANSFORM_H_
