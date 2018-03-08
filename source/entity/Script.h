#ifndef SST_SCRIPT_H_
#define SST_SCRIPT_H_

#include <string>
#include <memory>

#include <PySpot.h>
#include <PySpotModule.h>
#include <PySpotTuple.h>

#include "Object.h"


PyMODINIT_FUNC InitPySpot();

namespace pst = pyspot;


namespace sunspot
{

class Entity;

class Script : public Object
{
  public:
	Script(const int id, std::string& name, Entity& entity);
	Script(Entity& entity);
	~Script() {}

	void Initialize();
	void Update(const float delta);

	static void Initialize(const std::wstring& scriptPath);

  private:
	static std::unique_ptr<pst::PySpot> pyspot;

	Entity& mEntity;
	pst::PySpotModule mModule;
	pst::PySpotTuple mArgs;
};


}


#endif // SST_SCRIPT_H_
