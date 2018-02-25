#include <iostream>

#include <Logger.h>

#include "Framebuffer.h"
#include "Texture.h"
#include "ShaderProgram.h"

using namespace sunspot;
namespace lst = logspot;


const std::string Framebuffer::tag{ "Framebuffer" };


Framebuffer::Framebuffer(const mst::Size& size)
:	mSize         { size }
,	mFbo          { 0 }
,	mColorTexture { 0 }
,	mDepthTexture { 0 }
,	mHeaderTexture{ 0 }
,	mRbo          { 0 }
{
	glGenFramebuffers(1, &mFbo); // Create a framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

	glGenTextures(1, &mColorTexture); // Create a texture for colors
	glBindTexture(GL_TEXTURE_2D, mColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mSize.width, mSize.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture, 0);

	glGenTextures(1, &mDepthTexture); // Create a texture for depth
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mSize.width, mSize.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

	SoilData header{ "data/stereoscopic/header.png" };
	glGenTextures(1, &mHeaderTexture); // Create a texture for header
	glBindTexture(GL_TEXTURE_2D, mHeaderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, header.getWidth(), header.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, header.getHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		lst::Logger::log.info("Stereoscopic Framebuffer [%dx%d]\n", mSize.width, mSize.height);
	}
	else
	{
		glDeleteTextures(1, &mHeaderTexture);
		glDeleteTextures(1, &mDepthTexture);
		glDeleteTextures(1, &mColorTexture);
		glDeleteFramebuffers(1, &mFbo);
		throw FramebufferException{ tag, "Creation failed" };
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Framebuffer::~Framebuffer()
{
	glDeleteTextures(1, &mHeaderTexture);
	glDeleteTextures(1, &mDepthTexture);
	glDeleteTextures(1, &mColorTexture);
	glDeleteFramebuffers(1, &mFbo);
	lst::Logger::log.info("Framebuffer: destroyed\n"); // TODO remove debug log
}


void Framebuffer::bindColorTexture(const ShaderProgram& shader) const
{
	glUniform1i(shader.getLocation("colorTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mColorTexture);
	glUniform1i(shader.getLocation("headerTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mHeaderTexture);
	glUniform2f(shader.getLocation("frameSize"),
		static_cast<GLfloat>(mSize.width), static_cast<GLfloat>(mSize.height));
}


void Framebuffer::bindDepthTexture(const ShaderProgram& shader) const
{
	glUniform1i(shader.getLocation("depthTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glUniform2f(shader.getLocation("frameSize"),
		static_cast<GLfloat>(mSize.width), static_cast<GLfloat>(mSize.height));
}
