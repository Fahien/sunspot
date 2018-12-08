#ifndef SST_INPUT_H_
#define SST_INPUT_H_

#include "mathspot/Math.h"

#include "sunspot/input/Type.h"
#include "sunspot/input/Key.h"
#include "sunspot/input/Action.h"

namespace sunspot
{

namespace input
{

class [[pyspot::export]] Input
{
  public:
	Type    type;
	Key     key;
	Action  action;
	mathspot::Vec2 position;
};

} // namespace input

} // namespace sunspot

#endif // SST_INPUTACTION_H_