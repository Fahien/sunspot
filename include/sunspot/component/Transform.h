#ifndef SST_TRANSFORM_H_
#define SST_TRANSFORM_H_

#include <mathspot/Math.h>


namespace sunspot
{

namespace component
{


class [[pyspot::export]] Transform
{
  public:
	int id;
	mathspot::Vec3 position;
	mathspot::Vec3 rotation;
	mathspot::Vec3 scale { 1.0f, 1.0f, 1.0f };
};


} // namespace component

} // namespace sunspot


#endif
