#include <iostream>

#include "Framebuffer.h"
#include "Texture.h"
#include "ShaderProgram.h"

using namespace sunspot;


const Logger Framebuffer::log{};

const std::string Framebuffer::tag{ "Framebuffer" };


Framebuffer::Framebuffer(const math::Size &size)
	: size_{ size }
	, fbo_{ 0 }
	, colorTexture_{ 0 }
	, depthTexture_{ 0 }
	, headerTexture_{ 0 }
	, rbo_{ 0 }
{
	glGenFramebuffers(1, &fbo_); // Create a framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

	glGenTextures(1, &colorTexture_); // Create a texture for colors
	glBindTexture(GL_TEXTURE_2D, colorTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size_.width, size_.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture_, 0);

	glGenTextures(1, &depthTexture_); // Create a texture for depth
	glBindTexture(GL_TEXTURE_2D, depthTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size_.width, size_.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_, 0);

	SoilData header{ "data/stereoscopic/header.png" };
	glGenTextures(1, &headerTexture_); // Create a texture for header
	glBindTexture(GL_TEXTURE_2D, headerTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, header.getWidth(), header.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, header.getHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		log.info("Stereoscopic Framebuffer [%dx%d]\n", size_.width, size_.height);
	}
	else {
		glDeleteTextures(1, &headerTexture_);
		glDeleteTextures(1, &depthTexture_);
		glDeleteTextures(1, &colorTexture_);
		glDeleteFramebuffers(1, &fbo_);
		throw FramebufferException{ tag, "Creation failed" };
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Framebuffer::~Framebuffer()
{
	glDeleteTextures(1, &headerTexture_);
	glDeleteTextures(1, &depthTexture_);
	glDeleteTextures(1, &colorTexture_);
	glDeleteFramebuffers(1, &fbo_);
	log.info("Framebuffer: destroyed\n"); // TODO remove debug log
}


void Framebuffer::bindColorTexture(const ShaderProgram *shader) const
{
	glUniform1i(shader->getLocation("colorTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexture_);
	glUniform1i(shader->getLocation("headerTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, headerTexture_);
	glUniform2f(shader->getLocation("frameSize"), size_.width, size_.height);
}


void Framebuffer::bindDepthTexture(const ShaderProgram *shader) const
{
	glUniform1i(shader->getLocation("depthTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture_);
	glUniform2f(shader->getLocation("frameSize"), size_.width, size_.height);
}
