#pragma once

#ifdef ANDROID
# include <EGL/egl.h>
# include <GLES3/gl3.h>
class ANativeWindow;
#else
# include <GL/glew.h>
# include <GLFW/glfw3.h>
#endif
