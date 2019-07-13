#include "sunspot/core/Animations.h"
#include "logspot/Log.h"

#include <algorithm>


namespace sunspot
{
void Animations::update( const float delta_time, gltfspot::Gltf& gltf ) const
{
	using namespace gltfspot;

	for ( auto& animation : gltf.get_animations() )
	{
		animation.time.current += delta_time;

		for ( auto& sampler : animation.samplers )
		{
			// get time for keyframes
			auto&              time = gltf.GetAccessors().at( sampler.input );
			std::vector<float> times( time.count );
			auto&              view   = gltf.GetBufferViews().at( time.bufferView );
			auto               offset = time.byteOffset + view.byteOffset;
			auto&              buffer = gltf.GetBuffer( view.buffer );
			std::memcpy( times.data(), &buffer[offset], time.count * sizeof( float ) );

			auto it = std::max_element( std::begin( times ), std::end( times ) );
			if ( it != std::end( times ) )
			{
				animation.time.max = std::max<float>( animation.time.max, *it );
			}
		}


		// Start from the beginning
		if ( animation.time.current >= animation.time.max )
		{
			// Reset time
			animation.time.current -= animation.time.max;
		}

		for ( size_t i = 0; i < animation.channels.size(); ++i )
		{
			auto& channel = animation.channels[i];

			auto  node    = channel.target.node;
			auto& sampler = animation.samplers.at( channel.sampler );
			// get time for keyframes
			auto&              time = gltf.GetAccessors().at( sampler.input );
			std::vector<float> times( time.count );
			auto&              view   = gltf.GetBufferViews().at( time.bufferView );
			auto               offset = time.byteOffset + view.byteOffset;
			auto&              buffer = gltf.GetBuffer( view.buffer );
			std::memcpy( times.data(), &buffer[offset], time.count * sizeof( float ) );

			// Find the keyframe
			size_t keyframe = 1;
			for ( size_t i = 1; i < time.count; ++i )
			{
				if ( animation.time.current > times[i] )
				{
					keyframe++;
				}
				else
				{
					break;
				}
			}

			// Start from the beginning
			if ( keyframe >= time.count )
			{
				keyframe = 1;
			}

			if ( animation.time.current >= times[keyframe - 1] && animation.time.current <= times[keyframe] )
			{
				// Normalize time for lerp
				float norm_time =
				    ( animation.time.current - times[keyframe - 1] ) / ( times[keyframe] - times[keyframe - 1] );
				logspot::Log::info( "norm[%f] chan[%f]", norm_time, animation.time.current );

				auto& values = gltf.GetAccessors().at( sampler.output );

				auto  view_index   = values.bufferView;
				auto& data_view    = gltf.GetBufferViews().at( view_index );
				auto  data_offset  = values.byteOffset + data_view.byteOffset;
				auto  buffer_index = data_view.buffer;
				auto& data_buffer  = gltf.GetBuffer( buffer_index );

				switch ( channel.target.path )
				{
					default:
						assert( false && "Animation path not supported" );
						break;
					case Gltf::Animation::Target::Path::Rotation:
					{
						std::vector<mathspot::Quat> quats( values.count );
						std::memcpy( quats.data(), &data_buffer[data_offset], values.count * sizeof( mathspot::Quat ) );
						node->rotation = mathspot::slerp( quats[keyframe - 1], quats[keyframe], norm_time );
						break;
					}
					case Gltf::Animation::Target::Path::Scale:
					{
						std::vector<mathspot::Vec3> scales( values.count );
						std::memcpy( scales.data(), &data_buffer[data_offset], values.count * sizeof( mathspot::Vec3 ) );
						node->scale = mathspot::lerp( scales[keyframe - 1], scales[keyframe], norm_time );
						break;
					}
					case Gltf::Animation::Target::Path::Translation:
					{
						std::vector<mathspot::Vec3> trans( values.count );
						std::memcpy( trans.data(), &data_buffer[data_offset], values.count * sizeof( mathspot::Vec3 ) );
						node->translation = mathspot::lerp( trans[keyframe - 1], trans[keyframe], norm_time );

						break;
					}
					case Gltf::Animation::Target::Path::Weights:
						break;
				}
			}
		}
	}
}

}  // namespace sunspot