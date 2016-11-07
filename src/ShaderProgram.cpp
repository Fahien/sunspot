#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const char *depth)
	: baseProgram_ {glCreateProgram()}
	, depthProgram_ {glCreateProgram()}
{
	GLchar *vertexSource {loadShaderSource("shader/base.vert")};
	GLuint vertexShader {compileShader(GL_VERTEX_SHADER, vertexSource)};

	GLchar *fragmentSource {loadShaderSource("shader/base.frag")};
	GLuint fragmentShader {compileShader(GL_FRAGMENT_SHADER, fragmentSource)};

	linkProgram(baseProgram_, vertexShader, fragmentShader);

	GLchar *depthSource {loadShaderSource(depth)};
	GLuint depthShader {compileShader(GL_FRAGMENT_SHADER, depthSource)};
	
	linkProgram(depthProgram_, vertexShader, depthShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(depthShader);
	free(vertexSource);
	free(fragmentSource);
	free(depthSource);

	// TODO Handle some errors like vertexShader == 0, baseProgram_ == 0, etc.

	std::cout << "ShaderProgram: created\n"; // TODO remove debug log
}


ShaderProgram::ShaderProgram(const char *vertex, const char *fragment)
	: baseProgram_ {glCreateProgram()}
	, depthProgram_ {glCreateProgram()}
{
	GLchar *vertexSource {loadShaderSource(vertex)};
	GLuint vertexShader {compileShader(GL_VERTEX_SHADER, vertexSource)};
	GLchar *fragmentSource {loadShaderSource(fragment)};
	GLuint fragmentShader {compileShader(GL_FRAGMENT_SHADER, fragmentSource)};
	linkProgram(baseProgram_, vertexShader, fragmentShader);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	free(vertexSource);
	free(fragmentSource);
	
	// TODO Handle some errors like vertexShader == 0, baseProgram_ == 0, etc.

	std::cout << "ShaderProgram: created\n"; // TODO remove debug log
}


ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(depthProgram_);
	glDeleteProgram(baseProgram_);
	std::cout << "ShaderProgram: destroyed\n"; // TODO remove debug log
}


void ShaderProgram::linkProgram(const GLuint program, const GLuint vertex, const GLuint fragment)
{
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) { // TODO should throw exception
		GLchar infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cerr << "Error linking shader program: " << infoLog << std::endl;
		fprintf(stderr, "Error linking shader program: %s\n", infoLog);
	}
	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
}


GLuint ShaderProgram::compileShader(const GLenum type, const GLchar *source)
{
	GLuint shader {glCreateShader(type)}; // Create a shader object
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) { // TODO should throw exception
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Error compiling " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader: " << infoLog << std::endl;
	}
	return shader;
}


GLchar *ShaderProgram::loadShaderSource(const char *path)
{
	FILE *file;
	fopen_s(&file, path, "r");
	if (file == nullptr) { // TODO should throw exception
		std::cerr << "Could not open shader file: " << path << std::endl;;
	}
	fseek(file, 0, SEEK_END); // Calculate length
	long length {ftell(file) + 1};
	fseek(file, 0, SEEK_SET);
	GLchar *source {static_cast<GLchar *>(malloc(length * sizeof(GLchar)))};
	fread(source, 1, length, file);
	if (ferror(file)) { // TODO should throw exception
		std::cerr << "Could not read shader file: " << path << std::endl;
	}
	source[length - 1] = 0;
	//std::cout << "Shader source: " << source << std::endl;
	fclose(file);
	return source;
}
