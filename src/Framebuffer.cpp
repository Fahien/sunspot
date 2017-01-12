#include <iostream>

#include "Framebuffer.h"
#include "Texture.h"
#include "ShaderProgram.h"

using namespace sunspot;


const std::string Framebuffer::tag{ "Framebuffer" };


Framebuffer::Framebuffer(const math::Size &size)
	: size_{ size }
	, fbo_{ 0 }
	, colorTexture_{ 0 }
	, depthTexture_{ 0 }
	, maskTexture_{ 0 }
	, headerTexture_{ 0 }
	, rbo_{ 0 }
{
	std::cout << "Framebuffer: creating\n"; // TODO remove debug log

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

	GLubyte mask[1][2][3] = { {{255, 255, 255}, {0, 0, 0}} };
	glGenTextures(1, &maskTexture_); // Create a texture for mask
	glBindTexture(GL_TEXTURE_2D, maskTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, mask); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	TextureData header{ "data/stereoscopic/header.bmp" };
	glGenTextures(1, &headerTexture_); // Create a texture for header
	glBindTexture(GL_TEXTURE_2D, headerTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, header.getWidth(), header.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, header.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Stereoscopic Framebuffer [" << size_.width << "x" << size_.height << "]\n";
	}
	else {
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
	glDeleteTextures(1, &headerTexture_);
	glDeleteTextures(1, &maskTexture_);
	glDeleteTextures(1, &depthTexture_);
	glDeleteTextures(1, &colorTexture_);
	glDeleteFramebuffers(1, &fbo_);
	std::cout << "Framebuffer: destroyed\n"; // TODO remove debug log
}


void Framebuffer::bindColorTexture(const ShaderProgram *shader) const
{
	glUniform1i(shader->getLocation("screenTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexture_);
	glUniform1i(shader->getLocation("maskTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, maskTexture_);
	glUniform1i(shader->getLocation("headerTexture"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, headerTexture_);
	glUniform2i(shader->getLocation("frameSize"), size_.width, size_.height);
}


void Framebuffer::bindDepthTexture(const ShaderProgram *shader) const
{
	glUniform1i(shader->getLocation("depthTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture_);
	glUniform1i(shader->getLocation("maskTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, maskTexture_);
	glUniform2i(shader->getLocation("frameSize"), size_.width, size_.height);
}
