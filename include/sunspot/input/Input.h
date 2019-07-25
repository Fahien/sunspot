#ifndef SST_INPUT_INPUT_H_
#define SST_INPUT_INPUT_H_

#define PYSPOT_EXPORT __attribute__( ( annotate( "pyspot" ) ) )

#include "mathspot/Math.h"

#include "sunspot/input/Type.h"
#include "sunspot/input/Key.h"
#include "sunspot/input/Action.h"

namespace sunspot::input
{

class PYSPOT_EXPORT Input
{
  public:
	Type   type;
	Key    key;
	Action action;

	mathspot::Vec2 position;
};

}  // namespace sunspot::input

#endif // SST_INPUT_INPUT_H_
