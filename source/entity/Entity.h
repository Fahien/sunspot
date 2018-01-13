#ifndef SST_ENTITY_H
#define SST_ENTITY_H

#include "TransformComponent.h"
#include "Script.h"


namespace sunspot
{


class Mesh;


class Entity
{
public:
	Entity();
	Entity(Mesh* mesh);
	~Entity() {}

	inline Mesh* GetMesh() { return mMesh; }
	inline void SetMesh(Mesh* mesh) { mMesh = mesh; }

	void Update(const float delta);

	float value = 5.0f;

private:
	Mesh*   mMesh;
	PySpotTransformComponent mTransform;
	Script mScript;

	friend class Script;
};


}

#endif // SST_ENTITY_H
