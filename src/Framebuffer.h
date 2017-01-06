#ifndef SST_FRAMEBUFFER_H
#define SST_FRAMEBUFFER_H

#include <stdexcept>
#include <string>

#include "Graphics.h"

class ShaderProgram;

namespace sunspot {

class FramebufferException : public std::runtime_error {
  public:
	FramebufferException(const std::string& tag, const std::string& message) : std::runtime_error(tag + ": " + message) {}
};


class Framebuffer {
  public:
	static const std::string tag;

	Framebuffer(const int width, const int height);
	~Framebuffer();

	inline GLsizei getWidth() const { return width_; }
	inline GLsizei getHeight() const { return height_; }
	inline void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo_); }
	inline void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void bindColorTexture(const ShaderProgram *program) const;
	void bindDepthTexture(const ShaderProgram *program) const;

  private:
	const GLsizei width_;
	const GLsizei height_;
	GLuint fbo_;
	GLuint colorTexture_;
	GLuint depthTexture_;
	GLuint maskTexture_;
	GLuint headerTexture_;
	GLuint rbo_;
};

}

#endif // SST_FRAMEBUFFER_H

