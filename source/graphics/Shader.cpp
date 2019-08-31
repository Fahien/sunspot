#include "sunspot/graphics/Shader.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

#ifdef ANDROID
#include <filespot/Asset.h>
namespace fst = filespot;
#endif


namespace sunspot::graphics::shader
{
const std::string vertex_shader_path   = "shaders/base.vert";
const std::string fragment_shader_path = "shaders/base.frag";


Program::Program()
    : Program{ vertex_shader_path, fragment_shader_path }
{
}


Program::Program( const std::string& vertex, const std::string& fragment )
    : program{ glCreateProgram() }
{
	Source vertex_source{ vertex };
	Source fragment_source{ fragment };

	GLuint vertex_shader   = compile( GL_VERTEX_SHADER, vertex_source );
	GLuint fragment_shader = compile( GL_FRAGMENT_SHADER, fragment_source );

	link( vertex_shader, fragment_shader );

	glDeleteShader( vertex_shader );
	glDeleteShader( fragment_shader );

	// TODO Handle some errors like vertexShader == 0, program == 0, etc.
}


Program::~Program()
{
	glDeleteProgram( program );
}


void Program::link( const GLuint vertex, const GLuint fragment )
{
	glAttachShader( program, vertex );
	glAttachShader( program, fragment );
	glLinkProgram( program );

	GLint success;
	glGetProgramiv( program, GL_LINK_STATUS, &success );
	if ( !success )
	{
		GLchar info_log[512];
		glGetProgramInfoLog( program, 512, nullptr, info_log );
		std::string message{ "Error linking shader program: " };
		throw shader::Exception{ message + info_log };
	}

	glDetachShader( program, vertex );
	glDetachShader( program, fragment );
}


GLuint Program::compile( const GLenum type, const Source& source )
{
	GLuint shader = glCreateShader( type );  // Create a shader object
	glShaderSource( shader, 1, &source.handle, nullptr );
	glCompileShader( shader );

	GLint success;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if ( !success )
	{
		GLchar info_log[512];
		glGetShaderInfoLog( shader, 512, nullptr, info_log );
		throw shader::Exception{ "Error compiling " + source.path + ": " + info_log };
	}

	return shader;
}


Source::Source( const std::string& p )
    : path{ p }
    , handle{ nullptr }
{
#ifdef ANDROID
	fst::Asset file{ p };
	char*      content{ file.GetContent() };
	size_t     length{ file.GetLength() };
	handle = new GLchar[length];
	memcpy( handle, content, length );
#else
#if WIN32
	FILE* file{};
	fopen_s( &file, path.c_str(), "r" );
#else
	FILE* file{ fopen( path.c_str(), "r" ) };
#endif
	if ( !file )
	{
		throw shader::Exception{ "Could not open shader file: " + path };
	}
	fseek( file, 0, SEEK_END );  // Calculate length
	long length{ ftell( file ) + 1 };
	fseek( file, 0, SEEK_SET );
	handle = new GLchar[length];
	fread( handle, 1, length, file );
	if ( ferror( file ) )
	{
		throw shader::Exception{ "Could not read shader file: " + path };
	}
	handle[length - 1] = 0;
	fclose( file );
#endif  // ANDROID
}


Source::~Source()
{
	if ( handle )
	{
		delete[] handle;
	}
}


}  // namespace sunspot::graphics::shader
