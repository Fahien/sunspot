#include <algorithm>

#include <filespot/Ifstream.h>
#include <logspot/Logger.h>
#include <MathSpot.h>

#include "core/Renderer.h"


namespace mst = mathspot;
namespace sst = sunspot;
namespace lst = logspot;


static mst::Size windowSize{ 960, 540 };

static float fov {  45.0f };
static float near{ 0.125f };
static float far { 128.0f };


sst::Renderer::Renderer(const std::string& db)
:	mLastFrameNs{ 0 }
,	mEglContext{ eglGetCurrentContext() }
,	mShaderProgram{ "shader/android.vert", "shader/android.frag" }
,	mCamera{ fov, static_cast<float>(windowSize.width) / windowSize.height, near, far }
,	mLight{ Color{18.0f, 18.0f, 18.0f} }
,	mDataSpot{ db }
,	mModelRepository{}
,	mEntityRepository{ mDataSpot, mModelRepository }
{
	mLight.SetPosition(0.0f, 16.0f, 0.0f);
	mCamera.setPosition(0.0f, -32.0f, 0.0f);
	mCamera.LookAt(0.0f, 0.0f, 0.0f);

	// Test database
	int width  = stoi(mDataSpot.GetConfigValue("window.width"));
	int height = stoi(mDataSpot.GetConfigValue("window.height"));
	lst::Logger::log.Info("DataSpot Size [%dx%d]", width, height);

	// Read a set of objects from dataspot
	constexpr size_t entitiesCount = 3;
	// For every object get the name
	for (size_t i{ 0 }; i < entitiesCount; ++i)
	{
		sst::Entity* pEntity{ mEntityRepository.LoadEntity(i+1) };
		lst::Logger::log.Info("Entity: %s", pEntity->GetName().c_str());
		Add(pEntity);
	}
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


sst::Renderer* sst::Renderer::New(const std::string& db)
{
	sst::Renderer* renderer{ new sst::Renderer{ db } };
	if (!renderer->init())
	{
		delete renderer;
		return nullptr;
	}
	return renderer;
}


void sst::Renderer::Add(Entity* pEntity)
{
	mEntities.push_back(pEntity);
	mCollision.Add(*pEntity);
}


bool sst::Renderer::init()
{
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
	// Calculate delta time in nanoseconds
	step();

	mCollision.Update();

	for (auto entity : mEntities)
	{
		entity->Update(mDelta);
	}

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
	checkGlError("Renderer::render");
}

static const float deltaMin = 0.0025f;

static const float deltaMax = 0.015625f;

void sst::Renderer::step()
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	auto nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;
	mDelta = (nowNs - mLastFrameNs) / 1000000000.0f;
	mDelta = std::max(std::min(deltaMin, mDelta), deltaMax);
	mLastFrameNs = nowNs;
}


void sst::Renderer::draw()
{
	mShaderProgram.use();
	mCamera.update(0.125f, mShaderProgram);
	mLight.Update(mShaderProgram);

	for (auto entity : mEntities)
	{
		if (entity->GetModel())
		{
			auto pModel = entity->GetModel();
			pModel->GetRenderer().Draw(mShaderProgram, &pModel->GetNode());
		}
	}
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
