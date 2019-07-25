#ifndef SST_COLLISIONS_H_
#define SST_COLLISIONS_H_

#include <unordered_map>

#include <hitspot/BoundingBox.h>
#include <hitspot/CollisionSystem.h>
#include <gltfspot/Gltf.h>

namespace hst = hitspot;

namespace sunspot
{
class Entity;


class Collisions
{
  public:
	/// @param e Entity to add to the collision system
	void add( Entity& e );

	/// @brief Updates collisions
	void update( gltfspot::Gltf::Scene& scene);
	void update( const gltfspot::Node& node, const mathspot::Mat4& transform = mathspot::Mat4::identity );

	void resolve();
  private:
	std::vector<gltfspot::Shape*> shapes = {};

	hst::CollisionSystem system{};
};


}  // namespace sunspot

#endif  // SST_COLLISIONS_H_
