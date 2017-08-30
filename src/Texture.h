#ifndef SST_TEXTURE_H
#define SST_TEXTURE_H

#include <stdexcept>
#include <string>
#include "Graphics.h"
#include "Logger.h"


namespace sunspot {


class TextureException : public GraphicException {
  public:
	TextureException(const std::string& tag, const std::string& message)
		: GraphicException{tag + ": " + message } {}
};


class SoilData {
  public:
	static const std::string tag;

	SoilData(const std::string &path);
	~SoilData();

	inline GLsizei getWidth() { return width_; }
	inline GLsizei getHeight() { return height_; }
	inline GLubyte *getHandle() { return handle_; }

private:
	GLsizei width_;
	GLsizei height_;
	GLubyte *handle_;
};


class TextureData {
  public:
	static const std::string tag;

	TextureData(const std::string &path);
	~TextureData();

	inline GLsizei &getWidth() { return width_; }
	inline GLsizei &getHeight() { return height_; }
	inline GLubyte *getData() { return data_; }

  private:
	GLsizei width_;
	GLsizei height_;
	GLubyte *data_;
};


enum TextureType { DIFFUSE, SPECULAR };
static const char *textureTypeNames[] = { "diffuse", "specular" };
const char *getTextureTypeName(const TextureType &type);


class Texture {
  public:
	const static Logger log;
	Texture(const std::string &path, const TextureType &type);
	~Texture();

	inline void release() const { glDeleteTextures(1, &id_); }

	inline GLuint &getId() { return id_; }
	inline TextureType &getType() { return type_; }

  private:
	GLuint id_;
	std::string name_;
	TextureType type_;
};


}

#endif // SST_TEXTURE_H
