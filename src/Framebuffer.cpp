#include <iostream>

#include "Framebuffer.h"
#include "Texture.h"
#include "ShaderProgram.h"

using namespace sunspot;


const std::string Framebuffer::tag{ "Framebuffer" };


Framebuffer::Framebuffer(const int width, const int height)
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

	GLubyte mask[1][2][4] = { {{255, 255, 255}, {0, 0, 0}} };
	glGenTextures(1, &maskTexture_); // Create a texture for mask
	glBindTexture(GL_TEXTURE_2D, maskTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, mask); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	TextureData header{ "shader/header" };
	glGenTextures(1, &headerTexture_); // Create a texture for header
	glBindTexture(GL_TEXTURE_2D, headerTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, header.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &rbo_); // Create a renderbuffer object
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer: created\n"; // TODO remove debug log
	}
	else {
		glDeleteRenderbuffers(1, &rbo_);
		glDeleteTextures(1, &headerTexture_);
		glDeleteTextures(1, &maskTexture_);
		glDeleteTextures(1, &depthTexture_);
		glDeleteTextures(1, &colorTexture_);
		glDeleteFramebuffers(1, &fbo_);
		throw FramebufferException{ tag, "Creation failed" };
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Framebuffer::~Framebuffer()
{
	glDeleteRenderbuffers(1, &rbo_);
	glDeleteTextures(1, &headerTexture_);
	glDeleteTextures(1, &maskTexture_);
	glDeleteTextures(1, &depthTexture_);
	glDeleteTextures(1, &colorTexture_);
	glDeleteFramebuffers(1, &fbo_);
	std::cout << "Framebuffer: destroyed\n"; // TODO remove debug log
}


void Framebuffer::bindTextures(const ShaderProgram *shader) const
{
	glUniform1i(shader->getLocation("screenTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexture_);
	glUniform1i(shader->getLocation("depthTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture_);
	glUniform1i(shader->getLocation("maskTexture"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, maskTexture_);
	glUniform1i(shader->getLocation("headerTexture"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, headerTexture_);
}

