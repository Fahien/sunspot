#include "sunspot/core/Collisions.h"
#include "sunspot/component/Collider.h"
#include "sunspot/component/Transform.h"

namespace sunspot
{


void Collisions::update( const gltfspot::Node& node, const mathspot::Mat4& transform )
{
	// Current transform
	auto temp_transform = node.matrix;
	temp_transform.Scale( node.scale );
	temp_transform.Rotate( node.rotation );
	temp_transform.Translate( node.translation );
	temp_transform = transform * temp_transform;

	for ( auto child : node.children )
	{
		update( *child, temp_transform );
	}

	// Save its shape
	if ( node.bounds )
	{
		node.bounds->set_matrix( temp_transform );
		shapes.emplace_back( node.bounds );
	}
}


void Collisions::update( gltfspot::Gltf::Scene& scene )
{
	shapes.clear();
	for ( auto node : scene.nodes )
	{
		update( *node );
	}

	resolve();
}


void Collisions::resolve()
{
	for ( size_t i = 0; i < shapes.size(); ++i )
	{
		auto box = shapes[i];

		for ( size_t j = i + 1; j < shapes.size(); ++j )
		{
			auto other        = shapes[j];
			auto is_colliding = box->is_colliding_with( *other );

			if ( box->intersects( *other ) )
			{
				if ( !is_colliding )
				{
					box->add_collision( *other );
					other->add_collision( *box );
					if ( box->start_colliding_with )
					{
						box->start_colliding_with( *other );
					}
					if ( other->start_colliding_with )
					{
						other->start_colliding_with( *box );
					}
				}

				if ( box->colliding_with )
				{
					box->colliding_with( *other );
				}
				if ( other->colliding_with )
				{
					other->colliding_with( *box );
				}
			}
			else if ( is_colliding )
			{
				box->remove_collision( *other );
				other->remove_collision( *box );
				if ( box->end_colliding_with )
				{
					box->end_colliding_with( *other );
				}
				if ( other->end_colliding_with )
				{
					other->end_colliding_with( *box );
				}
			}
		}
	}
}


}  // namespace sunspot
