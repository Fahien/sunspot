#ifndef SST_INPUTACTION_H_
#define SST_INPUTACTION_H_

namespace sunspot
{

namespace input
{
	
enum class [[pyspot::export]] Action
{
	RELEASE = 0,
	PRESS   = 1,
	REPEAT = 2
};

}

}

#endif // SST_INPUTACTION_H_
