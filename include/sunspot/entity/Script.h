#ifndef SST_SCRIPT_H_
#define SST_SCRIPT_H_

#include <memory>
#include <string>

#include <pyspot/Interpreter.h>
#include <pyspot/Module.h>
#include <pyspot/Tuple.h>

#include "sunspot/entity/Object.h"
#include "sunspot/input/Input.h"


PyMODINIT_FUNC InitPySpot();

namespace pst = pyspot;


namespace sunspot
{
class Entity;

class Script : public Object
{
  public:
	Script( const int id, std::string& name, Entity& entity );
	Script( Entity& entity );

	virtual ~Script() = default;

	void initialize();
	void handle( const input::Input& input );
	void collide( Entity& other );
	void update( const float delta );

	static void initialize( const std::string& script_path );

  private:
	static std::unique_ptr<pst::Interpreter> interpreter;

	Entity&     entity;
	pst::Module module;
	pst::Tuple  args{ 1 };
};


}  // namespace sunspot


#endif  // SST_SCRIPT_H_
