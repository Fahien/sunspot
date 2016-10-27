#include <iostream>
#include <cstdlib>

#include "Framebuffer.h"


Framebuffer::Framebuffer(const unsigned width, const unsigned height)
	: fbo_ {0}
	, colorTexture_ {0}
	, depthTexture_ {0}
	, maskTexture_ {0}
	, headerTexture_ {0}
	, rbo_ {0}
{
	glGenFramebuffers(1, &fbo_); // Create a framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

	glGenTextures(1, &colorTexture_); // Create a texture for colors
	glBindTexture(GL_TEXTURE_2D, colorTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture_, 0);

	glGenTextures(1, &depthTexture_); // Create a texture for depth
	glBindTexture(GL_TEXTURE_2D, depthTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_, 0);

	GLubyte mask[1][2][4] = { {{0,0,0}, {255, 255, 255}} };
	glGenTextures(1, &maskTexture_); // Create a texture for mask
	glBindTexture(GL_TEXTURE_2D, maskTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, mask); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLubyte *header = loadHeader();
	glGenTextures(1, &headerTexture_); // Create a texture for header
	glBindTexture(GL_TEXTURE_2D, headerTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, header);
	free(header); // Release data
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &rbo_); // Create a renderbuffer object
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer -> created\n"; // TODO remove debug log
	}
	else { // TODO throw an exception
		std::cerr << "Framebuffer -> creation failed\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


GLubyte *Framebuffer::loadHeader()
{
	FILE *file {fopen("shader/header.bmp", "rb")};
	if (file == nullptr) { // TODO throw an exception
		std::cerr << "Framebuffer -> Could not open header bitmap\n";
	}

	fseek(file, 14, SEEK_CUR);

	unsigned hlen;
	size_t nread {fread(&hlen, 4, 1, file)};
	if (nread != 1) { // TODO throw an exception
		std::cerr << "Framebuffer -> Could not read header from bitmap file\n";
	}

	unsigned width; // Read width
	nread = fread(&width, 4, 1, file);
	if (nread != 1) { // TODO throw an exception
		std::cerr << "Framebuffer -> Could not read width from bitmap file\n";
	}

	unsigned height; // Read height;
	nread = fread(&height, 4, 1, file);
	if (nread != 1) { // TODO throw an exception
		std::cerr << "Framebuffer -> Could not read height from bitmap file\n";
	}

	unsigned short planes; // Read planes
	nread = fread(&planes, 2, 1, file);
	if (nread != 1) { // TODO throw an exception
		std::cerr << "Framebuffer -> Could not read planes from bitmap file\n";
	}

	unsigned short bpp; // Read bit per pixel
	nread = fread(&bpp, 2, 1, file);
	if (nread != 1) {
		std::cerr << "Framebuffer -> Could not read bpp from bitmap file\n";
	}
	if (bpp != 24) {
		std::cerr << "Framebuffer -> Bpp is not 24: " << bpp << std::endl;
	}

	// TODO remove debug log
	std::cout << "Framebuffer -> Bitmap header[" << hlen << "], size[" << width << "x" << height
		<< "], planes[" << planes << "], bpp[" << bpp << "]\n";

	fseek(file, 24, SEEK_CUR);

	unsigned size {width * height * 3}; // Allocate memory for bitmap data
	GLubyte *data {static_cast<GLubyte *>(malloc(size))};
	if (!data) { // TODO throw an exception
		std::cerr << "Framebuffer -> Could not allocate " << size << " memory for bitmap data\n";
	}
	nread = fread(data, size, 1, file); // Read bitmap data
	if (nread <= 0) { // TODO throw an exception
		std::cerr << "Framebuffer -> Could not read bitmap data: nread[" << nread << "]\n";
	}
	for (unsigned i {0}; i < size; i += 3) { // Adjust red and blue
		GLubyte temp = data[i];
		data[i] = data[i+2];
		data[i+2] = temp;
	}

	return data;
}


Framebuffer::~Framebuffer()
{
	glDeleteRenderbuffers(1, &rbo_);
	glDeleteTextures(1, &headerTexture_);
	glDeleteTextures(1, &maskTexture_);
	glDeleteTextures(1, &depthTexture_);
	glDeleteTextures(1, &colorTexture_);
	glDeleteFramebuffers(1, &fbo_);
	std::cout << "Framebuffer -> destroyed\n"; // TODO remove debug log
}
