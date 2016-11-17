#ifndef SST_TEXTURE_H
#define SST_TEXTURE_H

#include <exception>
#include "Graphics.h"


class TextureException : public std::runtime_error {
public:
	TextureException(const std::string& tag, const std::string& message) : std::runtime_error(tag + ": " + message) {}
};


class Texture {
  public:
	static const std::string tag;

	Texture(const std::string &path);
	~Texture();

	inline GLubyte *getData() { return data_; }

  private:
	GLubyte *data_;
};

#endif // SST_TEXTURE_H