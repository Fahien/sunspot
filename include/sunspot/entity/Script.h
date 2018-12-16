#ifndef SST_SCRIPT_H_
#define SST_SCRIPT_H_

#include <string>
#include <memory>

#include <pyspot/Interpreter.h>
#include <pyspot/Module.h>
#include <pyspot/Tuple.h>

#include "sunspot/input/Input.h"
#include "sunspot/entity/Object.h"


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
	void Handle(const input::Input& input);
	void Collide(Entity& other);
	void Update(const float delta);

	static void Initialize(const std::string& scriptPath);

  private:
	static std::unique_ptr<pst::Interpreter> interpreter;

	Entity& mEntity;
	pst::Module mModule;
	pst::Tuple mArgs{ 1 };
};


}


#endif // SST_SCRIPT_H_
