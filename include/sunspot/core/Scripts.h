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
	std::vector<Script*> get_active_scripts() const;

	void set_gltf( gltfspot::Gltf& g );

	void handle( input::Input& i );

	void update( gltfspot::Gltf& g, float delta );


  private:

	gltfspot::Gltf* gltf = nullptr;

	std::vector<Script> scripts;
};


}  // namespace sunspot


#endif  // SST_SCRIPTS_H_