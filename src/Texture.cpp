#include <iostream>
#include <cstdio>

#include "Texture.h"

using namespace sunspot;


const char *sunspot::getTextureTypeName(const TextureType &type)
{
	return textureTypeNames[type];
}


Texture::Texture(const std::string &path, const TextureType &type)
	: id_{ 0 }
	, name_{ path }
	, type_{ type }
{
	TextureData data{ path };
	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data.getWidth(), data.getHeight(),
		0, GL_RGB, GL_UNSIGNED_BYTE, data.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	std::cout << "Texture: created " << name_ << std::endl; // TODO remove debug log
}


Texture::~Texture()
{
	std::cout << "Texture: destroyed " << name_ << std::endl; // TODO remove debug log
}


const std::string TextureData::tag{ "TextureData" };


TextureData::TextureData(const std::string &path)
	: width_{}
	, height_{}
	, data_{nullptr}
{
#ifdef WIN32
	FILE *file{};
	fopen_s(&file, (path).c_str(), "rb");
#else
	FILE *file {fopen((path).c_str(), "rb")};
#endif
	if (file == nullptr) { throw TextureException{ tag, "Could not open header bitmap for " + path }; }

	fseek(file, 14, SEEK_CUR);

	unsigned hlen;
	size_t nread{ fread(&hlen, 4, 1, file) };
	if (nread != 1) {
		fclose(file);
		throw TextureException{ tag, "Could not read header from bitmap file" };
	}

	nread = fread(&width_, 4, 1, file); // Read width
	if (nread != 1) {
		fclose(file);
		throw TextureException{ tag, "Could not read width from bitmap file" };
	}

	nread = fread(&height_, 4, 1, file); // Read height;
	if (nread != 1) {
		fclose(file);
		throw TextureException{ tag, "Could not read height from bitmap file" };
	}

	unsigned short planes; // Read planes
	nread = fread(&planes, 2, 1, file);
	if (nread != 1) {
		fclose(file);
		throw TextureException{ tag, "Could not read planes from bitmap file" };
	}

	unsigned short bpp; // Read bit per pixel
	nread = fread(&bpp, 2, 1, file);
	if (nread != 1) {
		fclose(file);
		throw TextureException{ tag, "Could not read bpp from bitmap file" };
	}
	if (bpp != 24) {
		fclose(file);
		std::string message{ "Bpp is not 24: " };
		message += static_cast<char>(bpp);
		throw TextureException{ tag, message };
	}

	std::cout << tag.c_str() << ": " << path.c_str() << ", header[" << hlen
		<< "], size[" << width_ << "x" << height_
		<< "], planes[" << planes << "], bpp[" << bpp << "]\n";

	fseek(file, 24, SEEK_CUR);

	int size{ width_ * height_ * 3 }; // Allocate memory for bitmap data
	data_ = static_cast<GLubyte *>(malloc(size));
	if (!data_) {
		fclose(file);
		std::string message{ "Could not not allocate " };
		message += size;
		message += " memory for bitmap data\n";
		throw TextureException{ tag, message};
	}
	nread = fread(data_, size, 1, file); // Read bitmap data
	if (nread <= 0) {
		fclose(file);
		std::string message{ "Could not read bitmap data: nread[" };
		message += static_cast<char>(nread);
		message += "]";
		throw TextureException{ tag, message };
	}
	for (int i{ 0 }; i < size; i += 3) { // Adjust red and blue
		GLubyte temp = data_[i];
		data_[i] = data_[i + 2];
		data_[i + 2] = temp;
	}
	fclose(file);
}


TextureData::~TextureData()
{
	if (data_ != nullptr) { free(data_); }
}
