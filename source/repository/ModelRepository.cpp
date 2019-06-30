#include <dataspot/Database.h>
#include <gltfspot/Gltf.h>
#include <stdexcept>

#include "repository/ModelRepository.h"

using namespace std;
using namespace sunspot;
using namespace dataspot;
using namespace gltfspot;


const string ModelRepository::kExt{ ".gltf" };
const string ModelRepository::kModelDir{ "model" };


ModelRepository::ModelRepository( const string& projectDir ) : mProjectDir{ projectDir + "/" + kModelDir }
{
}


ModelRepository::~ModelRepository()
{
}


component::Model& ModelRepository::get_model( const int id, const string& path, const string& name )
{
	/*
	// Check whether the model has already been loaded
	auto itModel = mModels.find( id );
	if ( itModel != mModels.end() )
	{
		return itModel->second;
	}

	// Otherwise check whether the Gltf has already been loaded
	GltfRenderer* pRenderer{ nullptr };
	auto          itRenderer = mRenderers.find( path );
	if ( itRenderer != mRenderers.end() )
	{
		pRenderer = &itRenderer->second;
	}
	else
	{
		// Construct the complete path
		string       modelPath{ mProjectDir + "/" + path };
		GltfRenderer renderer{ Gltf::Load( modelPath ) };
		// Store it into the model map
		auto itRenderer = mRenderers.emplace( path, move( renderer ) );
		if ( itRenderer.second )
		{
			pRenderer = &itRenderer.first->second;
		}
	}

	// Find the nodes
	for ( auto& node : pRenderer->GetGltf().GetNodes() )
	{
		if ( name == "" || node.name == name )
		{
			// Create a Model component
			component::Model model{ node, *pRenderer };
			auto             it = mModels.emplace( id, model );
			if ( it.second )
			{
				return it.first->second;
			}
		}
	}
	*/

	std::string msg{ "Node not found: " };
	msg += name;
	throw runtime_error{ msg.c_str() };
}
