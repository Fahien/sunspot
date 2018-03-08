#ifndef SST_ENTITY_H_
#define SST_ENTITY_H_

#include "Transform.h"
#include "Script.h"
#include "Object.h"

#include <memory>

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

	inline PySpotTransform* GetTransform() { return mTransform; }
	inline void SetTransform(PySpotTransform* transform);

	inline Script* GetScript() { return mScript; }
	inline void SetScript(Script* script);

	void Update(const float delta);

private:
	std::shared_ptr<Mesh> mMesh;
	PySpotTransform*      mTransform = nullptr;
	Script*               mScript    = nullptr;

	friend class Script;
};

void Entity::SetTransform(PySpotTransform* transform)
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
