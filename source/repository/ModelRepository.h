#ifndef SST_MODELREPOSITORY_H_
#define SST_MODELREPOSITORY_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "sunspot/component/Model.h"
#include "view/GltfRenderer.h"


namespace sunspot
{
class ModelRepository
{
  public:
	static const std::string kExt;
	static const std::string kModelDir;

	ModelRepository( const std::string& projectDir = "." );
	~ModelRepository();

	component::Model& get_model( const int id, const std::string& path, const std::string& name = "" );

  private:
	const std::string                             mProjectDir;
	std::unordered_map<int, component::Model>     mModels;
	std::unordered_map<std::string, GltfRenderer> mRenderers;
};

}  // namespace sunspot

#endif  // SST_MODELREPOSITORY_H_
