#ifndef SST_ENTITY_H_
#define SST_ENTITY_H_

#include "pyspot/component/Transform.h"
#include "entity/Script.h"
#include "entity/Object.h"

#include <memory>

namespace pst = pyspot;

namespace sunspot
{


class Mesh;


class Entity : public Object
{
public:
	Entity();
	Entity(int id, std::string& name);

	~Entity();

	inline std::shared_ptr<Mesh>& GetMesh() { return mMesh; }
	inline void SetMesh(std::shared_ptr<Mesh>& mesh) { mMesh = mesh; }

	inline pst::component::Transform* GetTransform() { return mTransform; }
	inline void SetTransform(pst::component::Transform* transform);

	inline Script* GetScript() { return mScript; }
	inline void SetScript(Script* script);

	void Update(const float delta);

private:
	std::shared_ptr<Mesh>      mMesh;
	pst::component::Transform* mTransform = nullptr;
	Script*                    mScript    = nullptr;

	friend class Script;
};

void Entity::SetTransform(pst::component::Transform* transform)
{
	if (mTransform == transform)
	{
		return;
	}

	if (mTransform)
	{
		delete mTransform;
	}

	mTransform = transform;
}

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
