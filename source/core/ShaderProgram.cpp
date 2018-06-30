#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <logspot/Logger.h>

#ifdef ANDROID
# include <filespot/AssetManager.h>
namespace fst = filespot;
#endif

#include "core/ShaderProgram.h"

using namespace sunspot;
namespace lst = logspot;


#define STR(s) #s
#define STRV(s) STR(s)

#define POS_ATTRIB 0
#define COLOR_ATTRIB 1
#define SCALEROT_ATTRIB 2
#define OFFSET_ATTRIB 3

static const char VERTEX_SHADER[]   = "shaders/base.vert";
static const char FRAGMENT_SHADER[] = "shaders/base.frag";

ShaderProgram::ShaderProgram(const char* depth)
:	mBaseProgram { glCreateProgram() }
,	mDepthProgram{ glCreateProgram() }
{
	ShaderSource vertexSource  { VERTEX_SHADER   };
	ShaderSource fragmentSource{ FRAGMENT_SHADER };

	GLuint vertexShader  { compileShader(GL_VERTEX_SHADER,   vertexSource) };
	GLuint fragmentShader{ compileShader(GL_FRAGMENT_SHADER, fragmentSource) };

	linkProgram(mBaseProgram, vertexShader, fragmentShader);

	ShaderSource depthSource{ depth };
	GLuint depthShader{ compileShader(GL_FRAGMENT_SHADER, depthSource) };

	linkProgram(mDepthProgram, vertexShader, depthShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(depthShader);

	// TODO Handle some errors like vertexShader == 0, mBaseProgram == 0, etc.

	lst::Logger::log.Info("ShaderProgram: created\n"); // TODO remove debug log
}


ShaderProgram::ShaderProgram()
:	ShaderProgram{ VERTEX_SHADER, FRAGMENT_SHADER }
{}


ShaderProgram::ShaderProgram(const char* vertex, const char* fragment)
:	mBaseProgram { glCreateProgram() }
,	mDepthProgram{ glCreateProgram() }
{
	ShaderSource vertexSource  { vertex   };
	ShaderSource fragmentSource{ fragment };

	GLuint vertexShader  { compileShader(GL_VERTEX_SHADER,   vertexSource)   };
	GLuint fragmentShader{ compileShader(GL_FRAGMENT_SHADER, fragmentSource) };

	linkProgram(mBaseProgram, vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// TODO Handle some errors like vertexShader == 0, mBaseProgram == 0, etc.

	lst::Logger::log.Info("ShaderProgram: created\n"); // TODO remove debug log
}


ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(mDepthProgram);
	glDeleteProgram(mBaseProgram);
	lst::Logger::log.Info("ShaderProgram: destroyed\n"); // TODO remove debug log
}


void ShaderProgram::linkProgram(const GLuint program, const GLuint vertex, const GLuint fragment)
{
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::string message{ "Error linking shader program: " };
		throw ShaderException{ message + infoLog };
	}
	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
}


GLuint ShaderProgram::compileShader(const GLenum type, const ShaderSource& source)
{
	GLuint shader{ glCreateShader(type) }; // Create a shader object
	glShaderSource(shader, 1, &source.handle, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::string message{ "Error compiling "};
		throw ShaderException{ message + source.path + ": " + infoLog };
	}
	return shader;
}


ShaderSource::ShaderSource(const char* p)
:	path{ p }
,	handle{ nullptr }
{
#ifdef ANDROID
	fst::Asset file{ fst::AssetManager::assets.Open(p) };
	char* content{ file.GetContent() };
	size_t length{ file.GetLength() };
	handle = static_cast<GLchar*>(malloc(length * sizeof(char)));
	memcpy(handle, content, length);
#else // other systems
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
	if (ferror(file))
	{
		std::string message{ "Could not read shader file: " };
		throw ShaderException{ message + path };
	}
	handle[length - 1] = 0;
	fclose(file);
#endif // ANDROID
}

ShaderSource::~ShaderSource()
{
	free(handle);
}
