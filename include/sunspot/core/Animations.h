#ifndef SST_ANIMATIONS_H_
#define SST_ANIMATIONS_H_

#include <gltfspot/Gltf.h>

namespace sunspot
{

class Animations
{
public:
	void update( float delta_time, gltfspot::Gltf& gltf ) const;

private:
};

}

#endif // SST_ANIMATIONS_H_
