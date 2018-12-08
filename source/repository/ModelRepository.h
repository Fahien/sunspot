#ifndef SST_MODELREPOSITORY_H_
#define SST_MODELREPOSITORY_H_

#include <memory>
#include <unordered_map>
#include <string>

#include "Mesh.h"
#include "WavefrontObject.h"
#include "view/GltfRenderer.h"
#include "sunspot/component/Model.h"



namespace sunspot
{

class ModelRepository
{
  public:
	static const std::string kExt;
	static const std::string kModelDir;

	ModelRepository(const std::string& projectDir = "");
	~ModelRepository();

	component::Model& GetModel(const int id, const std::string& path, const std::string& name);

  private:
	const std::string mProjectDir;
	std::unordered_map<int, component::Model> mModels;
	std::unordered_map<std::string, GltfRenderer> mRenderers;
};

}

#endif // SST_MODELREPOSITORY_H_
