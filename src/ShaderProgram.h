#ifndef SST_SHADERPROGRAM_H
#define SST_SHADERPROGRAM_H

#include <stdexcept>
#include "Graphics.h"


namespace sunspot {


class ShaderException : public std::runtime_error {
  public:
	ShaderException(const std::string& message) : std::runtime_error(message) {}
};


class ShaderSource {
  public:
	ShaderSource(const char *path);
	~ShaderSource();

	const char *path;
	GLchar *handle;
};


class ShaderProgram {
  public:
	ShaderProgram(const char *depth);
	ShaderProgram(const char *vertex, const char *fragment);
	~ShaderProgram();

	inline GLuint getLocation(const char *name) const { return glGetUniformLocation(baseProgram_, name); }

	inline void use() const { glUseProgram(baseProgram_); }
	inline void setDepth() const { glUseProgram(depthProgram_); }

  private:
	GLuint compileShader(const GLenum type, const ShaderSource &source);
	void linkProgram(const GLuint program, const GLuint vertex, const GLuint fragment);

	GLuint baseProgram_;
	GLuint depthProgram_;
};


}

#endif // SST_SHADERPROGRAM_H
