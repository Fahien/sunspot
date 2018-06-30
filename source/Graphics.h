#ifndef SST_GRAPHICS_H_
#define SST_GRAPHICS_H_

#ifdef ANDROID
# include <GLES3/gl3.h>
#else
# define GLEW_STATIC
# include <GL/glew.h>
# include <GLFW/glfw3.h>
#endif

#include <stdexcept>

namespace sunspot
{

class GraphicException : public std::runtime_error {
public:
	GraphicException(const std::string& message) : std::runtime_error(message) {}
};

}


#endif // SST_GRAPHICS_H_
