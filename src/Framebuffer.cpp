#include <iostream>

#include "Framebuffer.h"


Framebuffer::Framebuffer(const unsigned width, const unsigned height)
	: fbo_ {0}
	, colorTexture_ {0}
	, depthTexture_ {0}
	, maskTexture_ {0}
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
	glGenTextures(1, &maskTexture_);
	glBindTexture(GL_TEXTURE_2D, maskTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, mask); 
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


Framebuffer::~Framebuffer()
{
	glDeleteRenderbuffers(1, &rbo_);
	glDeleteTextures(1, &maskTexture_);
	glDeleteTextures(1, &depthTexture_);
	glDeleteTextures(1, &colorTexture_);
	glDeleteFramebuffers(1, &fbo_);
	std::cout << "Framebuffer -> destroyed\n"; // TODO remove debug log
}
