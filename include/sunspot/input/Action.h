#ifndef SST_INPUT_ACTION_H_
#define SST_INPUT_ACTION_H_

#include "sunspot/util/Export.h"

namespace sunspot::input
{

enum class PYSPOT_EXPORT Action
{
	RELEASE = 0, PRESS = 1, REPEAT = 2
};

}  // namespace sunspot::input

#endif  // SST_INPUT_ACTION_H_
