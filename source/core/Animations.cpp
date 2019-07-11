#include "sunspot/core/Animations.h"
#include "logspot/Log.h"

namespace sunspot
{
void Animations::update( const float delta_time, gltfspot::Gltf& gltf ) const
{
	using namespace gltfspot;

	for ( auto& animation : gltf.get_animations() )
	{
		for ( size_t i = 0; i < animation.channels.size(); ++i )
		{
			auto& channel = animation.channels[i];
			channel.time += delta_time / 4.0f;

			auto  node    = channel.target.node;
			auto& sampler = animation.samplers.at( channel.sampler );
			// get time for keyframes
			auto& time   = gltf.GetAccessors().at( sampler.input );
			std::vector<float> times(time.count);
			auto& view = gltf.GetBufferViews().at( time.bufferView );
			auto offset = time.byteOffset + view.byteOffset;
			auto& buffer = gltf.GetBuffer( view.buffer );
			std::memcpy(times.data(), &buffer[offset], time.count * sizeof(float));

			// Find the keyframe
			size_t keyframe = 1;
			for (size_t i = 1; i < time.count; ++i)
			{
				if (channel.time > times[i])
				{
					keyframe++;
				}
			}

			// Start from the beginning
			if (keyframe >= time.count)
			{
				keyframe = 1;
				// Reset channel time
				channel.time -= times[time.count - 1];
				channel.time = 0.0f;
			}

			// Normalize time for lerp
			float norm_time = (channel.time - times[keyframe-1]) / times[keyframe];
			logspot::Log::info("channel.time: %f", channel.time);
			logspot::Log::info("keyframe: %d", keyframe);

			auto& values = gltf.GetAccessors().at( sampler.output );

			switch ( channel.target.path )
			{
				default:
					assert( false && "Animation path not supported" );
					break;
				case Gltf::Animation::Target::Path::Rotation:
				{
					auto  offset     = values.byteOffset;
					auto  view_index = values.bufferView;
					auto& view       = gltf.GetBufferViews().at( view_index );
					offset += view.byteOffset;
					auto           buffer_index = view.buffer;
					auto&          buffer       = gltf.GetBuffer( buffer_index );
					std::vector<mathspot::Quat> quats(values.count);
					std::memcpy( quats.data(), &buffer[offset], values.count * sizeof(mathspot::Quat) );

					node->rotation.x = (1.0f - norm_time) * quats[keyframe - 1].x + norm_time * quats[keyframe].x;
					node->rotation.y = (1.0f - norm_time) * quats[keyframe - 1].y + norm_time * quats[keyframe].y;
					node->rotation.z = (1.0f - norm_time) * quats[keyframe - 1].z + norm_time * quats[keyframe].z;
					node->rotation.w = (1.0f - norm_time) * quats[keyframe - 1].w + norm_time * quats[keyframe].w;

					break;
				}
				case Gltf::Animation::Target::Path::Scale:

					break;
				case Gltf::Animation::Target::Path::Translation:

					break;
				case Gltf::Animation::Target::Path::Weights:
					break;
			}
		}
	}
}

}  // namespace sunspot