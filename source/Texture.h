#ifndef SST_TEXTURE_H_
#define SST_TEXTURE_H_

#include <stdexcept>
#include <string>
#include <Graphics.h>

#ifdef ANDROID
# include <filespot/Asset.h>
namespace fst = filespot;
#endif


namespace sunspot {


class TextureException : public GraphicException
{
public:
	TextureException(const std::string& tag, const std::string& message)
		: GraphicException{tag + ": " + message } {}
};


class SoilData
{
public:
	static const std::string tag;

	SoilData(const std::string& path);
	~SoilData();

	inline GLsizei  getWidth ()   { return mWidth;    }
	inline GLsizei  getHeight()   { return mHeight;   }
	inline int      getChannels() { return mChannels; }
	inline GLubyte* getHandle()   { return mHandle;   }

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


class Texture {
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


}

#endif // SST_TEXTURE_H_
