#ifndef SST_SHADERPROGRAM_H
#define SST_SHADERPROGRAM_H

#include "Graphics.h"


class ShaderProgram
{
  public:
	ShaderProgram(const char *depth);
	ShaderProgram(const char *vertex, const char *fragment);
	~ShaderProgram();

	inline void setBase() const { glUseProgram(baseProgram_); }
	inline void setDepth() const { glUseProgram(depthProgram_); }
  private:
	GLchar *loadShaderSource(const char *path);
	GLuint compileShader(const GLenum type, const GLchar *source);
	void linkProgram(const GLuint program, const GLuint vertex, const GLuint fragment);

	GLuint baseProgram_;
	GLuint depthProgram_;
};

#endif // SST_SHADERPROGRAM_H
