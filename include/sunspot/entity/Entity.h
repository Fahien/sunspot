#ifndef SST_ENTITY_H_
#define SST_ENTITY_H_

#include <memory>
#include <optional>
#include <string>
#include <typeindex>
#include <vector>

#include <gltfspot/Gltf.h>

#include "sunspot/component/Transform.h"
#include "sunspot/entity/Object.h"
#include "sunspot/entity/Script.h"
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


}  // namespace component


class Entity : public Object
{
  public:
	Entity();
	Entity( int id, const std::string& name );

	~Entity();

	template <typename T>
	bool has();

	template <typename T>
	std::optional<std::reference_wrapper<T>> get();

	template <typename T>
	void add( T& );

	// component::Model* GetModel() { return mModel; }
	// void SetModel(component::Model* model);

	component::Collider* GetCollider()
	{
		return mCollider;
	}
	void SetCollider( component::Collider* mCollider );

	component::Transform& GetTransform()
	{
		return mTransform;
	}
	void SetTransform( component::Transform& transform );

	component::Rigidbody* GetRigidbody()
	{
		return mRigidbody;
	}
	void SetRigidbody( component::Rigidbody* rigidbody );

	Script* GetScript()
	{
		return mScript;
	}
	inline void SetScript( Script* script );

	void Handle( const input::Input& input );
	void Collide( Entity& other );
	void Update( const float delta );

  private:
	// component::Model*     mModel     { nullptr };
	component::Collider*  mCollider{ nullptr };
	component::Transform  mTransform{};
	component::Rigidbody* mRigidbody{ nullptr };
	Script*               mScript{ nullptr };

	std::unordered_map<std::type_index, Object*> m_Components{};

	friend class Script;
};


template <typename T>
bool Entity::has()
{
	static_assert( std::is_base_of<Object, T>(), "Component type is not subclass of Object" );
	auto index = std::type_index{ typeid( T ) };
	return m_Components.find( index ) != m_Components.end();
}

template <typename T>
std::optional<std::reference_wrapper<T>> Entity::get()
{
	static_assert( std::is_base_of<Object, T>(), "Component type is not subclass of Object" );
	auto index = std::type_index{ typeid( T ) };
	auto it    = m_Components.find( index );
	if ( it != m_Components.end() )
	{
		if ( auto pComponent = reinterpret_cast<T*>( it->second ) )
		{
			return *pComponent;
		}
	}
	return std::nullopt;
}


template <typename T>
void Entity::add( T& object )
{
	static_assert( std::is_base_of<Object, T>(), "Component type is not subclass of Object" );
	auto index          = std::type_index{ typeid( T ) };
	m_Components[index] = &object;
}


void Entity::SetScript( Script* script )
{
	if ( mScript == script )
	{
		return;
	}

	if ( mScript )
	{
		delete mScript;
	}

	mScript = script;
}

}  // namespace sunspot

#endif  // SST_ENTITY_H_
