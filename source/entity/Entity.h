#ifndef SST_ENTITY_H_
#define SST_ENTITY_H_

#include "Transform.h"
#include "Script.h"
#include "Object.h"


namespace sunspot
{


class Mesh;


class Entity : public Object
{
public:
	Entity();
	Entity(Mesh* mesh);
	Entity(int id, std::string& name);

	~Entity();

	inline Mesh* GetMesh() { return mMesh; }
	inline void SetMesh(Mesh* mesh) { mMesh = mesh; }

	inline PySpotTransform& GetTransform() { return *mTransform; }
	inline void SetTransform(PySpotTransform* transform);

	void Update(const float delta);

private:
	Mesh*            mMesh      = nullptr;
	PySpotTransform* mTransform = nullptr;
	Script*          mScript    = nullptr;

	friend class Script;
};

void Entity::SetTransform(PySpotTransform* transform)
{
	if (!mTransform)
	{
		delete mTransform;
	}

	mTransform = transform;
}

}

#endif // SST_ENTITY_H_
