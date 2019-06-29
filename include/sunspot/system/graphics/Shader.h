#pragma once

#include <stdexcept>

#include "sunspot/system/graphics/Gl.h"


namespace sunspot::graphics::shader
{


class Exception : public std::runtime_error
{
  public:
	Exception( const std::string& message ) : std::runtime_error( message ) {}
};

class Source
{
  public:
	Source( const char* path );
	~Source();

	const char* path;
	GLchar* handle;
};

class Program
{
  public:
	Program(const char* depth);
	Program();
	Program(const char* vertex, const char* fragment);
	~Program();

	inline GLuint GetLocation( const char* name ) const { return glGetUniformLocation( m_BaseProgram, name ); }

	void Use()      const { glUseProgram( m_BaseProgram ); }
	void SetDepth() const { glUseProgram( m_DepthProgram ); }

  private:
	GLuint Compile(const GLenum type, const Source& source);
	void Link(const GLuint program, const GLuint vertex, const GLuint fragment);

	GLuint m_BaseProgram;
	GLuint m_DepthProgram;
};


} // namespace shader
