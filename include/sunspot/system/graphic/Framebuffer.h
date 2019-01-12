#ifndef SST_FRAMEBUFFER_H
#define SST_FRAMEBUFFER_H

#include <stdexcept>
#include <string>
#include <mathspot/Math.h>

#include "sunspot/system/graphic/Shader.h"

namespace mst = mathspot;


namespace sunspot::graphic
{


class FramebufferException : public std::runtime_error {
public:
	FramebufferException(const std::string& tag, const std::string& message) : std::runtime_error(tag + ": " + message) {}
};


class Framebuffer {
public:
	static const std::string tag;

	Framebuffer(const mst::Size& size);
	~Framebuffer();

	inline GLsizei getWidth()  const { return mSize.width;  }
	inline GLsizei getHeight() const { return mSize.height; }

	inline void bind()   const { glBindFramebuffer(GL_FRAMEBUFFER, mFbo); }
	inline void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0);    }

	void bindColorTexture(const shader::Program& program) const;
	void bindDepthTexture(const shader::Program& program) const;

private:
	const mst::Size mSize;
	GLuint mFbo;
	GLuint mColorTexture;
	GLuint mDepthTexture;
	GLuint mHeaderTexture;
};


}


#endif // SST_FRAMEBUFFER_H