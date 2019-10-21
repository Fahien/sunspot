#ifndef SST_SHADER_H_
#define SST_SHADER_H_

#include <stdexcept>
#include <string>

#include "sunspot/graphics/gl/Gl.h"


namespace sunspot::graphics::shader
{
class Exception : public std::runtime_error
{
  public:
	Exception( const std::string& message )
	    : std::runtime_error( message )
	{
	}
};

class Source
{
  public:
	Source( const std::string& path );
	~Source();

	std::string path;
	GLchar*     handle = nullptr;
};

class Program
{
  public:
	Program();
	Program( const std::string& vertex, const std::string& fragment );
	~Program();

	GLuint get_location( const std::string& name ) const { return glGetUniformLocation( program, name.c_str() ); }

	void use() const { glUseProgram( program ); }

  private:
	GLuint compile( GLenum type, const Source& source );

	void link( GLuint vertex, GLuint fragment );

	GLuint program;
};


}  // namespace sunspot::graphics::shader

#endif  // SST_SHADER_H_