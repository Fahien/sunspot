#include <stdexcept>
#include <DataSpot.h>

#include "Ifstream.h"
#include "ModelRepository.h"

using namespace std;
using namespace sunspot;


const std::string ModelRepository::kExt{ ".obj" };
const std::string ModelRepository::kModelDir{ "model/" };


ModelRepository::ModelRepository(const dst::DataSpot& data, const std::string& projectDir)
:	mData{ data }
,	mProjectDir{ projectDir + kModelDir }
,	mMeshes{}
,	mModels{}
{}


ModelRepository::~ModelRepository()
{}


std::shared_ptr<Mesh> ModelRepository::GetMesh(const int id, const string& path, const string& name)
{
	// Check whether the mesh is already cached
	auto meshIt = mMeshes.find(id);
	if (meshIt != mMeshes.end())
	{
		return meshIt->second;
	}

	// Otherwise check whether the model has already been loaded
	auto it = mModels.find(path);
	shared_ptr<WavefrontObject> obj;
	if (it != mModels.end())
	{
		obj = it->second;
	}
	else
	{
		// Construct the complete path
		string modelPath{ mProjectDir + path + "/" + path + kExt };
		Ifstream is{ modelPath };
		if (!is.is_open())
		{
			throw runtime_error{ "Could not load " + modelPath };
		}
		obj = shared_ptr<WavefrontObject>(new WavefrontObject{});
		// Load the object from the input stream
		is >> *obj;
		// Store it into the model map
		mModels.emplace(path, obj);
	}

	// TODO do I want a copy?
	auto mesh = obj->GetMesh(name);
	mMeshes.emplace(id, mesh);
	return mesh;
}
