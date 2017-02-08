#include <iostream>
#include <cstdio>
#include <SOIL/SOIL.h>

#include "Texture.h"

using namespace sunspot;


const Logger Texture::log{};


const char *sunspot::getTextureTypeName(const TextureType &type)
{
	return textureTypeNames[type];
}


Texture::Texture(const std::string &path, const TextureType &type)
	: id_{ 0 }
	, name_{ path }
	, type_{ type }
{
	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);
	SoilData data{ path };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data.getWidth(), data.getHeight(),
		0, GL_RGB, GL_UNSIGNED_BYTE, data.getHandle());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	log.info("Texture: created %s\n", name_.c_str()); // TODO remove debug log
}


Texture::~Texture()
{
	log.info("Texture: destroyed %s\n", name_.c_str()); // TODO remove debug log
}


const std::string SoilData::tag{ "SoilData" };


SoilData::SoilData(const std::string &path)
	: width_{ 0 }
	, height_{ 0 }
	, handle_{ nullptr }
{
	handle_ = SOIL_load_image(path.c_str(), &width_, &height_, 0, SOIL_LOAD_RGB);
	if (handle_ == nullptr || handle_ == 0) {
		throw TextureException{ tag, "Could not load " + path + ": " + SOIL_last_result() };
	}

	Texture::log.info("SoilData: %s %s\n", path.c_str(), SOIL_last_result()); // TODO remove debug log
}


SoilData::~SoilData()
{
	if (handle_ != nullptr && handle_ != 0) { SOIL_free_image_data(handle_); }
}


const std::string TextureData::tag{ "TextureData" };


TextureData::TextureData(const std::string &path)
	: width_{ 0 }
	, height_{ 0 }
	, data_{ nullptr }
{
#ifdef WIN32
	FILE *file{};
	fopen_s(&file, (path).c_str(), "rb");
#else
	FILE *file{ fopen((path).c_str(), "rb") };
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
		throw TextureException{ tag, message };
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
