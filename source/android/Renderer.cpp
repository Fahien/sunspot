#include <filespot/Ifstream.h>
#include <logspot/Logger.h>
#include <MathSpot.h>

#include <sunspot/android/Renderer.h>


namespace mst = mathspot;
namespace sst = sunspot;
namespace lst = logspot;


static mst::Size windowSize{ 960, 540 };

static float fov {  45.0f };
static float near{ 0.125f };
static float far { 128.0f };


sst::Renderer::Renderer()
:	mLastFrameNs{ 0 }
,	mEglContext{ eglGetCurrentContext() }
,	mShaderProgram{ "shader/android.vert", "shader/android.frag" }
,	mCamera{ fov, static_cast<float>(windowSize.width) / windowSize.height, near, far }
,	mLight{ Color{10.0f, 10.0f, -10.0f} }
{
	mCamera.setPosition(5.0f, 20.0f, -40.0f);
}


sst::Renderer::~Renderer()
{
	// The destructor may be called after the context has already been
	// destroyed, in which case our objects have already been destroyed

	// If the context exists, it must be current. This only happens when
	// we're cleaning up after a failed init()
	if (eglGetCurrentContext() != mEglContext)
	{
		return;
	}
}


sst::Renderer* sst::Renderer::createRenderer()
{
	sst::Renderer* renderer{ new sst::Renderer };
	if (!renderer->init())
	{
		delete renderer;
		return nullptr;
	}
	return renderer;
}


bool sst::Renderer::init()
{
	mLight.SetPosition(2.0f, 2.0f, -4.0f);

	//fst::Ifstream os{ "model/dragon/recchion_drago.obj" };
	//os >> mScene;

	lst::Logger::log.Info("Using OpenGL ES 3.0 renderer");
	return true;
}


void sst::Renderer::resize(int w, int h)
{
	mLastFrameNs = 0;
	glViewport(0, 0, w, h);
}


void sst::Renderer::render()
{
	step();

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
	checkGlError("Renderer::render");
}


void sst::Renderer::step()
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	auto nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;
	mLastFrameNs = nowNs;
}


void sst::Renderer::draw()
{
	mShaderProgram.use();
	mCamera.update(0.125f, mShaderProgram);
	mLight.Update(mShaderProgram);
	//mScene.draw(mShaderProgram);
}


bool sst::Renderer::checkGlError(const char* funcName)
{
	GLint err = glGetError();
	if (err != GL_NO_ERROR)
	{
		lst::Logger::log.Error("GL error after %s(): 0x%08x\n", funcName, err);
		return true;
	}
	return false;
}