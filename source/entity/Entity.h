#ifndef SST_ENTITY_H_
#define SST_ENTITY_H_

#include <memory>
#include <Gltf.h>

#include "component/Model.h"
#include "entity/Script.h"
#include "entity/Object.h"
#include "input/Input.h"


namespace pst = pyspot;
namespace gst = gltfspot;

namespace sunspot
{

class Mesh;

namespace component
{

class Transform;

}


class Entity : public Object
{
public:
	Entity();
	Entity(int id, std::string& name);

	~Entity();

	inline Model* GetModel() { return mModel; }
	inline void SetModel(Model* model);

	inline component::Transform* GetTransform() { return mTransform; }
	void SetTransform(component::Transform* transform);

	inline Script* GetScript() { return mScript; }
	inline void SetScript(Script* script);

	void Update(const float delta, const input::Key key);

private:
	Model*                mModel     { nullptr };
	component::Transform* mTransform { nullptr };
	Script*               mScript    { nullptr };

	friend class Script;
};



void Entity::SetModel(Model* model)
{
	if (mModel == model)
	{
		return;
	}

	if (mModel)
	{
		delete mModel;
	}

	mModel = model;
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
