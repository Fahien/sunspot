#include "sunspot/system/graphic/Shader.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <logspot/Log.h>

#ifdef ANDROID
# include <filespot/Asset.h>
namespace fst = filespot;
#endif


using namespace sunspot;
namespace lst = logspot;


#define STR(s) #s
#define STRV(s) STR(s)

#define POS_ATTRIB 0
#define COLOR_ATTRIB 1
#define SCALEROT_ATTRIB 2
#define OFFSET_ATTRIB 3


namespace sunspot::graphic::shader
{

static const char VERTEX_SHADER[]   = "shaders/base.vert";
static const char FRAGMENT_SHADER[] = "shaders/base.frag";

Program::Program(const char* depth)
:	m_BaseProgram { glCreateProgram() }
,	m_DepthProgram{ glCreateProgram() }
{
	Source vertexSource  { VERTEX_SHADER   };
	Source fragmentSource{ FRAGMENT_SHADER };

	GLuint vertexShader  { Compile(GL_VERTEX_SHADER,   vertexSource) };
	GLuint fragmentShader{ Compile(GL_FRAGMENT_SHADER, fragmentSource) };

	Link(m_BaseProgram, vertexShader, fragmentShader);

	Source depthSource{ depth };
	GLuint depthShader{ Compile(GL_FRAGMENT_SHADER, depthSource) };

	Link(m_DepthProgram, vertexShader, depthShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(depthShader);

	// TODO Handle some errors like vertexShader == 0, m_BaseProgram == 0, etc.

	lst::Log::info("Program: created\n"); // TODO remove debug log
}


Program::Program()
:	Program{ VERTEX_SHADER, FRAGMENT_SHADER }
{}


Program::Program(const char* vertex, const char* fragment)
:	m_BaseProgram { glCreateProgram() }
,	m_DepthProgram{ glCreateProgram() }
{
	Source vertexSource  { vertex   };
	Source fragmentSource{ fragment };

	GLuint vertexShader  { Compile(GL_VERTEX_SHADER,   vertexSource)   };
	GLuint fragmentShader{ Compile(GL_FRAGMENT_SHADER, fragmentSource) };

	Link(m_BaseProgram, vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// TODO Handle some errors like vertexShader == 0, m_BaseProgram == 0, etc.

	lst::Log::info("Program: created\n"); // TODO remove debug log
}


Program::~Program()
{
	glDeleteProgram(m_DepthProgram);
	glDeleteProgram(m_BaseProgram);
	lst::Log::info("Program: destroyed\n"); // TODO remove debug log
}


void Program::Link(const GLuint program, const GLuint vertex, const GLuint fragment)
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
		throw shader::Exception{ message + infoLog };
	}
	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
}


GLuint Program::Compile(const GLenum type, const Source& source)
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
		throw shader::Exception{ message + source.path + ": " + infoLog };
	}
	return shader;
}


Source::Source(const char* p)
:	path{ p }
,	handle{ nullptr }
{
#ifdef ANDROID
	fst::Asset file{ p };
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
		throw shader::Exception{ message + path };
	}
	fseek(file, 0, SEEK_END); // Calculate length
	long length {ftell(file) + 1};
	fseek(file, 0, SEEK_SET);
	handle = static_cast<GLchar *>(malloc(length * sizeof(GLchar)));
	fread(handle, 1, length, file);
	if (ferror(file))
	{
		std::string message{ "Could not read shader file: " };
		throw shader::Exception{ message + path };
	}
	handle[length - 1] = 0;
	fclose(file);
#endif // ANDROID
}

Source::~Source()
{
	free(handle);
}


} // namespace sunspot::graphic::shader
