#ifndef SST_MODELREPOSITORY_H_
#define SST_MODELREPOSITORY_H_

#include <memory>
#include <map>
#include <string>

#include "Mesh.h"
#include "WavefrontObject.h"


namespace dataspot
{
	class DataSpot;
}

namespace dst = dataspot;


namespace sunspot
{

class ModelRepository
{
  public:
	static const std::string kExt;
	static const std::string kModelDir;

	ModelRepository(const dst::DataSpot& data, const std::string& projectDir);
	~ModelRepository();

	std::shared_ptr<Mesh> GetMesh(const int id, const std::string& path, const std::string& name);

  private:
	const dst::DataSpot& mData;
	const std::string mProjectDir;
	std::map<const int, std::shared_ptr<Mesh>> mMeshes;
	std::map<std::string, std::shared_ptr<WavefrontObject>> mModels;
};

}

#endif // SST_MODELREPOSITORY_H_
