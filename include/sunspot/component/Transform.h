#ifndef SST_TRANSFORM_H_
#define SST_TRANSFORM_H_

#include <mathspot/Math.h>
#include <sunspot/entity/Object.h>

#define PYSPOT_EXPORT __attribute__( ( annotate( "pyspot" ) ) )

namespace sunspot::component
{
class PYSPOT_EXPORT Transform : public Object
{
  public:
	int            id;
	mathspot::Vec3 position;
	mathspot::Vec3 rotation;
	mathspot::Vec3 scale{ 1.0f, 1.0f, 1.0f };
};


}  // namespace sunspot::component


#endif  // SST_TRANSFORM_H_
