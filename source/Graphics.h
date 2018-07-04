#ifndef SST_GRAPHICS_H_
#define SST_GRAPHICS_H_

#ifdef ANDROID
# include <EGL/egl.h>
# include <GLES3/gl3.h>
class ANativeWindow;
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


#ifdef ANDROID

class Graphics
{
  public:
	~Graphics();

	static Graphics& Get(ANativeWindow* window = nullptr);

	bool Swap();
	void DestroySurface();
	void Destroy();

  private:
	Graphics(ANativeWindow* window);

	void init(ANativeWindow* window);
	void initEGLSurface();
	void initEGLContext();

	ANativeWindow* mWindow;
	EGLDisplay mDisplay;
	EGLSurface mSurface;
	EGLConfig  mConfig;
	EGLContext mContext;
};

#endif


}


#endif // SST_GRAPHICS_H_
