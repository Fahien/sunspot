#include <iostream>
#include <cstdio>

#include "Texture.h"


const std::string Texture::tag{ "Texture" };


Texture::Texture(const std::string &path)
	: data_{nullptr}
{
	FILE *file {fopen((path + ".bmp").c_str(), "rb")};
	if (file == nullptr) { throw TextureException{ tag, "Could not open header bitmap" }; }

	fseek(file, 14, SEEK_CUR);

	unsigned hlen;
	size_t nread{ fread(&hlen, 4, 1, file) };
	if (nread != 1) {
		fclose(file);
		throw TextureException{ tag, "Could not read header from bitmap file" };
	}

	unsigned width; // Read width
	nread = fread(&width, 4, 1, file);
	if (nread != 1) {
		fclose(file);
		throw TextureException{ tag, "Could not read width from bitmap file" };
	}

	unsigned height; // Read height;
	nread = fread(&height, 4, 1, file);
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

	std::cout << tag.c_str() << ": " << path.c_str() << ", header[" << hlen << "], size[" << width << "x" << height
		<< "], planes[" << planes << "], bpp[" << bpp << "]\n";

	fseek(file, 24, SEEK_CUR);

	unsigned size{ width * height * 3 }; // Allocate memory for bitmap data
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
	for (unsigned i{ 0 }; i < size; i += 3) { // Adjust red and blue
		GLubyte temp = data_[i];
		data_[i] = data_[i + 2];
		data_[i + 2] = temp;
	}
	fclose(file);
}


Texture::~Texture()
{
	if (data_ != nullptr) { free(data_); }
}
