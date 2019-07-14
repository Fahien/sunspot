#ifndef SST_SCRIPT_H_
#define SST_SCRIPT_H_

#include <memory>
#include <string>

#include <pyspot/Interpreter.h>
#include <pyspot/Module.h>
#include <pyspot/Tuple.h>

#include <gltfspot/Gltf.h>

#include "sunspot/entity/Object.h"
#include "sunspot/input/Input.h"


PyMODINIT_FUNC InitPySpot();

namespace pst = pyspot;

namespace sunspot
{
class Entity;

class Script
{
  public:
	Script( const std::string& uri );

	void init( gltfspot::Node& n );
	void handle( gltfspot::Node& n, const input::Input& i );
	void collide( gltfspot::Node& a, gltfspot::Node& b );
	void update( gltfspot::Node& n, const float delta );

	static void init( const std::string& script_path = "." );

  private:
	static std::unique_ptr<pst::Interpreter> interpreter;

	pst::Module module;
	pst::Tuple  args = { 2 };
};


}  // namespace sunspot


#endif  // SST_SCRIPT_H_
