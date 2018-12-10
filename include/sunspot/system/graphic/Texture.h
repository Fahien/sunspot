#pragma once

#include <stdexcept>
#include <string>

#ifdef ANDROID
# include <filespot/Asset.h>
namespace fst = filespot;
#endif

#include "sunspot/system/graphic/Gl.h"
#include "sunspot/system/graphic/Exception.h"


namespace sunspot::graphic
{


class TextureException : public graphic::Exception
{
  public:
	TextureException(const std::string& tag, const std::string& message)
		: graphic::Exception{tag + ": " + message } {}
};


class SoilData
{
  public:
	static const std::string tag;

	SoilData(const std::string& path);
	~SoilData();

	GLsizei  getWidth ()   { return mWidth;    }
	GLsizei  getHeight()   { return mHeight;   }
	int      getChannels() { return mChannels; }
	GLubyte* getHandle()   { return mHandle;   }

  private:
	GLsizei  mWidth;
	GLsizei  mHeight;
	int      mChannels;
	GLubyte* mHandle;
#ifdef ANDROID
	fst::Asset    mAsset;
#endif
};


class TextureData
{
  public:
	static const std::string tag;

	TextureData(const std::string& path);
	~TextureData();

	inline GLsizei& getWidth()  { return mWidth;  }
	inline GLsizei& getHeight() { return mHeight; }
	inline GLubyte* getData()   { return mData;   }

  private:
	GLsizei  mWidth;
	GLsizei  mHeight;
	GLubyte* mData;
};


enum TextureType { DIFFUSE, SPECULAR };
static const char* textureTypeNames[] = { "diffuse", "specular" };
const char* getTextureTypeName(const TextureType& type);


class Texture
{
  public:
	Texture(const std::string& path, const TextureType& type);
	~Texture();

	inline void release() const { glDeleteTextures(1, &mId); }

	inline GLuint&      getId()   { return mId;   }
	inline TextureType& getType() { return mType; }

  private:
	GLuint      mId;
	std::string mName;
	TextureType mType;
};


} // namespace sunspot::graphic
