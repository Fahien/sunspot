#ifndef SST_RIGIDBODY_H_
#define SST_RIGIDBODY_H_

#include <mathspot/Math.h>
#include <sunspot/entity/Object.h>

#define PYSPOT_EXPORT __attribute__( ( annotate( "pyspot" ) ) )

namespace sunspot::component
{
struct PYSPOT_EXPORT Rigidbody
{
	int            id;
	mathspot::Vec3 velocity;
};


}  // namespace sunspot::component

#endif  // SST_RIGIDBODY_H_
