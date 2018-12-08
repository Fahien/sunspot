#ifndef SST_INPUTTYPE_H_
#define SST_INPUTTYPE_H_

namespace sunspot
{
	
namespace input
{

enum class [[pyspot::export]] Type
{
	INVALID= 0,
	KEY    = 1,
	MOTION = 2
};

} // namespace input

} // namespace sunspot

#endif // SST_INPUTACTION_H_