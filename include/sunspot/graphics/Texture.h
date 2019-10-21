#pragma once

#include <stdexcept>
#include <string>

#ifdef ANDROID
#include <filespot/Asset.h>
namespace fst = filespot;
#endif
#include <gltfspot/Gltf.h>

#include "sunspot/graphics/Exception.h"
#include "sunspot/graphics/gl/Gl.h"


namespace sunspot::graphics
{
class TextureException : public graphics::Exception
{
  public:
	TextureException( const std::string& tag, const std::string& message )
	    : graphics::Exception{ tag + ": " + message }
	{
	}
};


class SoilData
{
  public:
	static const std::string tag;

	SoilData( const std::string& path );
	SoilData( const gltfspot::Gltf::BufferView& buffer_view, gltfspot::Gltf& gltf );
	~SoilData();

	GLsizei  getWidth() const { return mWidth; }
	GLsizei  getHeight() const { return mHeight; }
	int      getChannels() const { return mChannels; }
	GLubyte* getHandle() const { return mHandle; }

  private:
	GLsizei  mWidth;
	GLsizei  mHeight;
	int      mChannels;
	GLubyte* mHandle;
#ifdef ANDROID
	fst::Asset mAsset;
#endif
};


class TextureData
{
  public:
	static const std::string tag;

	TextureData( const std::string& path );
	~TextureData();

	inline GLsizei& getWidth() { return mWidth; }
	inline GLsizei& getHeight() { return mHeight; }
	inline GLubyte* getData() { return mData; }

  private:
	GLsizei  mWidth;
	GLsizei  mHeight;
	GLubyte* mData;
};


enum TextureType
{
	DIFFUSE,
	SPECULAR
};

static const char* textureTypeNames[] = { "diffuse", "specular" };
const char*        getTextureTypeName( const TextureType& type );


class Texture
{
  public:
	Texture( const std::string& path, const TextureType& type );
	Texture( const SoilData& data, const TextureType& type );

	Texture( Texture&& other );

	~Texture();

	GLuint      get_id() const { return id; }
	TextureType get_type() const { return type; }

  private:
	GLuint      id = 0;
	std::string name;
	TextureType type;
};


}  // namespace sunspot::graphics
