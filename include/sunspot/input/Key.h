#ifndef SST_INPUTKEY_H_
#define SST_INPUTKEY_H_

namespace sunspot
{

namespace input
{

enum class [[pyspot::export]] Key
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

} // namespace input

} // namespace sunspot

#endif // SST_INPUTACTION_H_