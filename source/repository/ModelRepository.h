#ifndef SST_MODELREPOSITORY_H_
#define SST_MODELREPOSITORY_H_

#include <memory>
#include <map>
#include <string>
#include <Gltf.h>

#include "Mesh.h"
#include "WavefrontObject.h"
#include "view/GltfRenderer.h"
#include "sunspot/component/Model.h"


namespace dataspot
{
	class DataSpot;
}

namespace dst = dataspot;
namespace gst = gltfspot;


namespace sunspot
{

class ModelRepository
{
  public:
	static const std::string kExt;
	static const std::string kModelDir;

	ModelRepository(const dst::DataSpot& data, const std::string& projectDir);
	~ModelRepository();

	component::Model& GetModel(const int id, const std::string& path, const std::string& name);

  private:
	const dst::DataSpot& mData;
	const std::string    mProjectDir;
	std::map<const int, component::Model> mModels {};
	std::map<std::string, GltfRenderer> mRenderers {};
};

}

#endif // SST_MODELREPOSITORY_H_
