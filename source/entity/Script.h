#ifndef SST_SCRIPT_H
#define SST_SCRIPT_H

#include <string>
#include <PySpot.h>
#include <PySpotModule.h>

PyMODINIT_FUNC InitPySpot();

namespace pst = pyspot;


namespace sunspot
{

class Entity;

class Script
{
public:
	static pst::PySpot pyspot;

	Script(Entity& entity);
	~Script() {}

	void Update(const float delta);

private:
	Entity& mEntity;
	pst::PySpotModule mModule;
};


}


#endif // SST_SCRIPT_H
