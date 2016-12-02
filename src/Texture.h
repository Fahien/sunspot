#ifndef SST_TEXTURE_H
#define SST_TEXTURE_H

#include <stdexcept>
#include <string>
#include "Graphics.h"


namespace sunspot {


class TextureException : public std::runtime_error {
  public:
	TextureException(const std::string& tag, const std::string& message)
		: std::runtime_error{tag + ": " + message } {}
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
	Texture(const std::string &path, const TextureType &type);
	~Texture();

	inline GLuint &getId() { return id_; }
	inline TextureType &getType() { return type_; }

  private:
	GLuint id_;
	TextureType type_;
};


}

#endif // SST_TEXTURE_H

