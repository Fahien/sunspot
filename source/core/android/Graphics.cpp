#include <android/native_window.h>
#include <logspot/Logger.h>

#include "Graphics.h"

using namespace sunspot;
namespace lst = logspot;


Graphics& Graphics::Get(ANativeWindow* window)
{
	static Graphics graphics{ window };

	if (window)
	{
		if (window != graphics.mWindow)
		{
			graphics.Destroy();
			graphics.init(window);
		}
	}

	return graphics;
}


Graphics::Graphics(ANativeWindow* window)
:	mWindow { nullptr }
,	mDisplay{ EGL_NO_DISPLAY }
,	mSurface{ EGL_NO_SURFACE }
,	mConfig {}
,	mContext{ EGL_NO_CONTEXT }
{
	init(window);
}


void Graphics::init(ANativeWindow* window)
{
	mWindow = window;

	initEGLSurface();

	initEGLContext();

	auto versionStr = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	if (strstr(versionStr, "OpenGL ES 3."))
	{
		lst::Logger::log.Info("glES 3 supported");
	}
}


Graphics::~Graphics()
{
	Destroy();
}


void Graphics::initEGLSurface()
{
	// InitEGLSurface
	mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(mDisplay, 0, 0);

	EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
		EGL_BLUE_SIZE,  8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE,   8,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};

	EGLint configCount;
	eglChooseConfig(mDisplay, attribs, &mConfig, 1, &configCount);

	if (!configCount)
	{
		// Fall back to 16bit depth buffer
		attribs[11] = 16;
		eglChooseConfig(mDisplay, attribs, &mConfig, 1, &configCount);

		if (!configCount)
		{
			lst::Logger::log.Error("Unable to retrieve EGL config");
		}
	}

	mSurface = eglCreateWindowSurface(mDisplay, mConfig, mWindow, nullptr);

	EGLint screenWidth;
	EGLint screenHeight;
	eglQuerySurface(mDisplay, mSurface, EGL_WIDTH,  &screenWidth);
	eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &screenHeight);
}


void Graphics::initEGLContext()
{
	const EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	mContext = eglCreateContext(mDisplay, mConfig, nullptr, context_attribs);

	if (eglMakeCurrent(mDisplay, mSurface, mSurface, mContext) == EGL_FALSE)
	{
		lst::Logger::log.Error("Cannot glMakeCurrent");
	}
}


bool Graphics::Swap()
{
	auto swapped = eglSwapBuffers(mDisplay, mSurface);
	if (!swapped)
	{
		auto err = eglGetError();
		if (err == EGL_BAD_SURFACE)
		{
			// Recreate surface
			initEGLSurface();
			// Still consider glContext is valid
			return true;
		}
		else if (err == EGL_CONTEXT_LOST || err == EGL_BAD_CONTEXT)
		{
			// Context has been lost!!
			//context_valid_ = false;
			Destroy();
			initEGLContext();
		}

		return false;
	}

	return EGL_SUCCESS;
}


void Graphics::DestroySurface()
{
	if (mSurface != EGL_NO_SURFACE)
	{
		eglDestroySurface(mDisplay, mSurface);
		mSurface = EGL_NO_SURFACE;
	}
}


void Graphics::Destroy()
{
	if (mDisplay != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (mContext != EGL_NO_CONTEXT)
		{
			eglDestroyContext(mDisplay, mContext);
		}

		DestroySurface();

		eglTerminate(mDisplay);
	}

	mDisplay = EGL_NO_DISPLAY;
	mContext = EGL_NO_CONTEXT;
	mSurface = EGL_NO_SURFACE;
	mWindow  = nullptr;
}