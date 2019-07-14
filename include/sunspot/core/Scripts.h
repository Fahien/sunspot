#ifndef SST_SCRIPTS_H_
#define SST_SCRIPTS_H_

#include "sunspot/entity/Script.h"


namespace gltfspot
{
class Gltf;
}


namespace sunspot
{
class Scripts
{
  public:
	void update( gltfspot::Gltf& g, float delta );

	void set_gltf( gltfspot::Gltf& g );

  private:

	gltfspot::Gltf* gltf = nullptr;

	std::vector<Script> scripts;
};


}  // namespace sunspot


#endif  // SST_SCRIPTS_H_