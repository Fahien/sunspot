#ifndef SST_SHADERPROGRAM_H
#define SST_SHADERPROGRAM_H

#include "Graphics.h"


class ShaderProgram
{
  public:
	ShaderProgram(const char *depth);
	ShaderProgram(const char *vertex, const char *fragment);
	~ShaderProgram();

	inline GLuint getLocation(const char *name) const { return glGetUniformLocation(baseProgram_, name); }

	inline void use() const { glUseProgram(baseProgram_); }
	inline void setDepth() const { glUseProgram(depthProgram_); }

	inline void setUniforms() const
	{
	
		GLint texLoc = glGetUniformLocation(baseProgram_, "screenTexture");
		glUniform1i(texLoc, 0);
		texLoc = glGetUniformLocation(baseProgram_, "maskTexture");
		glUniform1i(texLoc, 1);
		texLoc = glGetUniformLocation(baseProgram_, "headerTexture");
		glUniform1i(texLoc, 2);
	}

  private:
	GLchar *loadShaderSource(const char *path);
	GLuint compileShader(const GLenum type, const GLchar *source);
	void linkProgram(const GLuint program, const GLuint vertex, const GLuint fragment);

	GLuint baseProgram_;
	GLuint depthProgram_;
};

#endif // SST_SHADERPROGRAM_H
