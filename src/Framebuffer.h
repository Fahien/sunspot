#ifndef SST_FRAMEBUFFER_H
#define SST_FRAMEBUFFER_H

#include <exception>

#include "Graphics.h"

class ShaderProgram;


class FramebufferException : public std::runtime_error {
public:
	FramebufferException(const std::string& tag, const std::string& message) : std::runtime_error(tag + ": " + message) {}
};


class Framebuffer {
  public:
	static const std::string tag;

	Framebuffer(const unsigned width, const unsigned height);
	~Framebuffer();

	inline void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo_); }
	inline void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void bindTextures(const ShaderProgram *program) const;

	inline void bindColorTexture() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexture_);
	}

	inline void bindDepthTexture() const
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthTexture_);
	}

	inline void bindMaskTexture() const
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, maskTexture_);
	}

	inline void bindHeaderTexture() const
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, headerTexture_);
	}

  private:
	GLuint fbo_;
	GLuint colorTexture_;
	GLuint depthTexture_;
	GLuint maskTexture_;
	GLuint headerTexture_;
	GLuint rbo_;
};

#endif // SST_FRAMEBUFFER_H
