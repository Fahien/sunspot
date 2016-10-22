#ifndef SST_FRAMEBUFFER_H
#define SST_FRAMEBUFFER_H

#include "Graphics.h"

class Framebuffer
{
  public:
	Framebuffer(const unsigned width, const unsigned height);
	~Framebuffer();

	inline void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo_); }
	inline void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	inline void bindColorTexture() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexture_);
	}

	inline void bindMaskTexture() const
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, maskTexture_);
	}
  private:
	GLuint fbo_;
	GLuint colorTexture_;
	GLuint depthTexture_;
	GLuint maskTexture_;
	GLuint rbo_;
};

#endif // SST_FRAMEBUFFER_H
