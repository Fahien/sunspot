#ifndef SST_INPUT_TYPE_H_
#define SST_INPUT_TYPE_H_

#include "sunspot/util/Export.h"

namespace sunspot::input
{

enum class PYSPOT_EXPORT Type
{
	INVALID= 0,
	KEY    = 1,
	MOTION = 2
};

} // namespace sunspot::input

#endif // SST_INPUT_TYPE_H_
