#include "sunspot/core/Scripts.h"

#include <gltfspot/Gltf.h>


namespace sunspot
{
void Scripts::set_gltf( gltfspot::Gltf& g )
{
	if ( gltf != &g )
	{
		gltf = &g;

		// Load scripts
		scripts.clear();

		scripts.reserve( g.get_scripts().size() );

		for ( auto& s : g.get_scripts() )
		{
			scripts.emplace_back( s.uri );
		}

		for ( auto node : g.GetScene()->nodes )
		{
			for ( auto script_index : node->scripts_indices )
			{
				auto& script = scripts[script_index];
				script.init( *node );
			}
		}
	}
}


void Scripts::update( gltfspot::Gltf& g, const float delta )
{
	set_gltf( g );

	for ( auto node : g.GetScene()->nodes )
	{
		for ( auto script_index : node->scripts_indices )
		{
			auto& script = scripts[script_index];
			script.update( *node, delta );
		}
	}
}

}  // namespace sunspot