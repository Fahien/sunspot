#ifndef SST_ENTITY_H_
#define SST_ENTITY_H_

#include <memory>
#include <Gltf.h>

#include "sunspot/entity/Script.h"
#include "sunspot/entity/Object.h"
#include "sunspot/input/Input.h"


namespace pst = pyspot;
namespace gst = gltfspot;

namespace sunspot
{

class Mesh;

namespace component
{

class Model;
class Collider;
class Transform;
class Rigidbody;


}


class Entity : public Object
{
public:
	Entity();
	Entity(int id, std::string& name);

	~Entity();

	component::Model* GetModel() { return mModel; }
	void SetModel(component::Model* model);

	component::Collider* GetCollider() { return mCollider; }
	void SetCollider(component::Collider* mCollider);

	component::Transform* GetTransform() { return mTransform; }
	void SetTransform(component::Transform* transform);

	component::Rigidbody* GetRigidbody() { return mRigidbody; }
	void SetRigidbody(component::Rigidbody* rigidbody);

	Script* GetScript() { return mScript; }
	inline void SetScript(Script* script);

	void Handle(const input::Input& input);
	void Collide(Entity& other);
	void Update(const float delta, const input::Input& input);

private:
	component::Model*     mModel     { nullptr };
	component::Collider*  mCollider  { nullptr };
	component::Transform* mTransform { nullptr };
	component::Rigidbody* mRigidbody { nullptr };
	Script*               mScript    { nullptr };

	friend class Script;
};



void Entity::SetScript(Script* script)
{
	if (mScript == script)
	{
		return;
	}

	if (mScript)
	{
		delete mScript;
	}

	mScript = script;
}

}

#endif // SST_ENTITY_H_
