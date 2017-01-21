#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "ShaderProgram.h"

using namespace sunspot;


ShaderProgram::ShaderProgram(const char *depth)
	: baseProgram_{ glCreateProgram() }
	, depthProgram_{ glCreateProgram() }
{
	ShaderSource vertexSource{ "shader/base.vert" };
	GLuint vertexShader{ compileShader(GL_VERTEX_SHADER, vertexSource) };

	ShaderSource fragmentSource{ "shader/base.frag" };
	GLuint fragmentShader{ compileShader(GL_FRAGMENT_SHADER, fragmentSource) };

	linkProgram(baseProgram_, vertexShader, fragmentShader);

	ShaderSource depthSource{ depth };
	GLuint depthShader{ compileShader(GL_FRAGMENT_SHADER, depthSource) };
	
	linkProgram(depthProgram_, vertexShader, depthShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(depthShader);

	// TODO Handle some errors like vertexShader == 0, baseProgram_ == 0, etc.

	std::cout << "ShaderProgram: created\n"; // TODO remove debug log
}


ShaderProgram::ShaderProgram(const char *vertex, const char *fragment)
	: baseProgram_{ glCreateProgram() }
	, depthProgram_{ glCreateProgram() }
{
	ShaderSource vertexSource{ vertex };
	GLuint vertexShader{ compileShader(GL_VERTEX_SHADER, vertexSource) };
	ShaderSource fragmentSource{ fragment };
	GLuint fragmentShader{ compileShader(GL_FRAGMENT_SHADER, fragmentSource) };
	linkProgram(baseProgram_, vertexShader, fragmentShader);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
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
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::string message{ "Error linking shader program: " };
		throw ShaderException{ message + infoLog };
	}
	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
}


GLuint ShaderProgram::compileShader(const GLenum type, const ShaderSource &source)
{
	GLuint shader{ glCreateShader(type) }; // Create a shader object
	glShaderSource(shader, 1, &source.handle, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::string message{ "Error compiling "};
		throw ShaderException{ message
			+ source.path + ": " + infoLog };
	}
	return shader;
}


ShaderSource::ShaderSource(const char *p)
	: path{ p }
	, handle{ nullptr }
{
#ifdef WIN32
	FILE *file{};
	fopen_s(&file, path, "r");
#else
	FILE *file {fopen(path, "r")};
#endif
	if (file == nullptr) {
		std::string message{ "Could not open shader file: " };
		throw ShaderException{ message + path };
	}
	fseek(file, 0, SEEK_END); // Calculate length
	long length {ftell(file) + 1};
	fseek(file, 0, SEEK_SET);
	handle = static_cast<GLchar *>(malloc(length * sizeof(GLchar)));
	fread(handle, 1, length, file);
	if (ferror(file)) {
		std::string message{ "Could not read shader file: " };
		throw ShaderException{ message + path };
	}
	handle[length - 1] = 0;
	fclose(file);
}

ShaderSource::~ShaderSource()
{
	free(handle);
}
