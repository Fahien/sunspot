#include "sunspot/graphics/Texture.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <SOIL.h>
#include <logspot/Log.h>

#include "sunspot/graphics/Gl.h"

namespace lst = logspot;

namespace sunspot::graphics
{
const char* getTextureTypeName( const TextureType& type )
{
	return textureTypeNames[type];
}


Texture::Texture( const std::string& path, const TextureType& type ) : mId{ 0 }, mName{ path }, mType{ type }
{
	glGenTextures( 1, &mId );
	glBindTexture( GL_TEXTURE_2D, mId );
	SoilData data{ path };
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, data.getWidth(), data.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
	              data.getHandle() );
	glGenerateMipmap( GL_TEXTURE_2D );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	lst::Log::info( "Texture: created %s", mName.c_str() );  // TODO remove debug log
}

Texture::Texture( const SoilData& data, const TextureType& type ) : mType{ type }
{
	glGenTextures( 1, &mId );
	glBindTexture( GL_TEXTURE_2D, mId );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, data.getWidth(), data.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
	              data.getHandle() );
	glGenerateMipmap( GL_TEXTURE_2D );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

Texture::Texture( Texture&& other ) : mId{ other.mId }, mName{ other.mName }, mType{ other.mType }
{
}

Texture::~Texture()
{
	// TODO Release MIPMAP
	lst::Log::info( "Texture: destroyed %s", mName.c_str() );  // TODO remove debug log
}


const std::string SoilData::tag{ "SoilData" };


SoilData::SoilData( const std::string& path ) : mWidth{ 0 }, mHeight{ 0 }, mChannels{ 0 }, mHandle
{
	nullptr
}
#ifdef ANDROID
, mAsset
{
	path
}
#endif
{
#ifdef ANDROID
	mHandle = SOIL_load_image_from_memory( reinterpret_cast<const unsigned char* const>( mAsset.GetContent() ),
	                                       mAsset.GetLength(), &mWidth, &mHeight, 0, SOIL_LOAD_RGB );
#else
	mHandle = SOIL_load_image( path.c_str(), &mWidth, &mHeight, &mChannels, SOIL_LOAD_RGB );
#endif
	if ( mHandle == nullptr || mHandle == 0 )
	{
		throw TextureException{ tag, "Could not load " + path + ": " + SOIL_last_result() };
	}

	lst::Log::info( "SoilData: %s %s", path.c_str(), SOIL_last_result() );  // TODO remove debug log
}

SoilData::SoilData( const gltfspot::Gltf::BufferView& buffer_view, gltfspot::Gltf& gltf )
{
	auto& buffer = gltf.GetBuffer( buffer_view.buffer );
	auto  first  = buffer.data() + buffer_view.byteOffset;
	mHandle = SOIL_load_image_from_memory( reinterpret_cast<const unsigned char* const>( first ), buffer_view.byteLength,
	                                       &mWidth, &mHeight, &mChannels, SOIL_LOAD_RGB );
	if ( mHandle == nullptr || mHandle == 0 )
	{
		throw TextureException{ tag, std::string{ "Could not load GLTF buffer: " } + SOIL_last_result() };
	}
}

SoilData::~SoilData()
{
	if ( mHandle != nullptr && mHandle != 0 )
	{
		SOIL_free_image_data( mHandle );
	}
}


const std::string TextureData::tag{ "TextureData" };


TextureData::TextureData( const std::string& path ) : mWidth{ 0 }, mHeight{ 0 }, mData{ nullptr }
{
#ifdef WIN32
	FILE* file{};
	fopen_s( &file, ( path ).c_str(), "rb" );
#else
	FILE* file{ fopen( ( path ).c_str(), "rb" ) };
#endif
	if ( file == nullptr )
	{
		throw TextureException{ tag, "Could not open header bitmap for " + path };
	}

	fseek( file, 14, SEEK_CUR );

	unsigned hlen;
	size_t   nread{ fread( &hlen, 4, 1, file ) };
	if ( nread != 1 )
	{
		fclose( file );
		throw TextureException{ tag, "Could not read header from bitmap file" };
	}

	nread = fread( &mWidth, 4, 1, file );  // Read width
	if ( nread != 1 )
	{
		fclose( file );
		throw TextureException{ tag, "Could not read width from bitmap file" };
	}

	nread = fread( &mHeight, 4, 1, file );  // Read height;
	if ( nread != 1 )
	{
		fclose( file );
		throw TextureException{ tag, "Could not read height from bitmap file" };
	}

	unsigned short planes;  // Read planes
	nread = fread( &planes, 2, 1, file );
	if ( nread != 1 )
	{
		fclose( file );
		throw TextureException{ tag, "Could not read planes from bitmap file" };
	}

	unsigned short bpp;  // Read bit per pixel
	nread = fread( &bpp, 2, 1, file );
	if ( nread != 1 )
	{
		fclose( file );
		throw TextureException{ tag, "Could not read bpp from bitmap file" };
	}
	if ( bpp != 24 )
	{
		fclose( file );
		std::string message{ "Bpp is not 24: " };
		message += static_cast<char>( bpp );
		throw TextureException{ tag, message };
	}

	std::cout << tag.c_str() << ": " << path.c_str() << ", header[" << hlen << "], size[" << mWidth << "x" << mHeight
	          << "], planes[" << planes << "], bpp[" << bpp << "]\n";

	fseek( file, 24, SEEK_CUR );

	int size{ mWidth * mHeight * 3 };  // Allocate memory for bitmap data
	mData = static_cast<GLubyte*>( malloc( size ) );
	if ( !mData )
	{
		fclose( file );
		std::string message{ "Could not not allocate " };
		message += size;
		message += " memory for bitmap data\n";
		throw TextureException{ tag, message };
	}
	nread = fread( mData, size, 1, file );  // Read bitmap data
	if ( nread <= 0 )
	{
		fclose( file );
		std::string message{ "Could not read bitmap data: nread[" };
		message += static_cast<char>( nread );
		message += "]";
		throw TextureException{ tag, message };
	}
	// Adjust red and blue
	for ( int i{ 0 }; i < size; i += 3 )
	{
		GLubyte temp = mData[i];
		mData[i]     = mData[i + 2];
		mData[i + 2] = temp;
	}
	fclose( file );
}


TextureData::~TextureData()
{
	if ( mData != nullptr )
	{
		free( mData );
	}
}


}  // namespace sunspot::graphics
