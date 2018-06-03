#ifndef SST_SCRIPT_H_
#define SST_SCRIPT_H_

#include <string>
#include <memory>

#include <pyspot/Interpreter.h>
#include <pyspot/Module.h>
#include <pyspot/Tuple.h>

#include "input/Input.h"
#include "entity/Object.h"


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
	void Update(const float delta, const input::Key key);

	static void Initialize(const std::wstring& scriptPath);

  private:
	static std::unique_ptr<pst::Interpreter> interpreter;

	Entity& mEntity;
	pst::Module mModule;
	pst::Tuple mArgs;
};


}


#endif // SST_SCRIPT_H_
