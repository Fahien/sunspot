#ifndef SST_INPUT_KEY_H_
#define SST_INPUT_KEY_H_

#include "sunspot/util/Export.h"

namespace sunspot::input
{

enum class PYSPOT_EXPORT Key
{
	NONE = 0,
	UP   = 1,
	LEFT = 2,
	DOWN = 3,
	RIGHT= 4,
	W    = 5,
	A    = 6,
	S    = 7,
	D    = 8,
	Q    = 9,
	E    = 10
};

} // namespace sunspot::input

#endif // SST_INPUT_KEY_H_
