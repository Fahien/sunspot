#include "sunspot/component/Rigidbody.h"
#include "sunspot/component/Transform.h"
#include "sunspot/input/Action.h"
#include "sunspot/input/Input.h"
#include "sunspot/input/Key.h"
#include "sunspot/input/Type.h"
#include "sunspot/core/Gui.h"
#include "mathspot/Math.h"
#include "gltfspot/Gltf.h"

namespace sunspot
{

struct PYSPOT_EXPORT Export
{
	gltfspot::Gltf* g;
	gltfspot::Node* n;
};

}
