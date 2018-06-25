#include <iostream>

#include "SunSpotConfig.h"
#ifdef SST_PROFILING
#include <chrono>
#endif

#include "view/GltfRenderer.h"
#include "Graphics.h"
#include "Window.h"
#include "core/ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "WavefrontObject.h"
#include "Quad.h"
#include "Framebuffer.h"
#include "entity/Entity.h"
#include "component/Model.h"

using namespace sunspot;


const std::string Window::tag{ "Window" };


Window::Window(const char* title, const mst::Size windowSize, const bool decorated, const bool stereoscopic)
:	mTitle{ title }
,	mWindowSize{ windowSize }
,	mMonitorSize{ windowSize }
,	mFrameSize{ windowSize }
,	mDecorated{ decorated }
,	mStereoscopic{ stereoscopic }
,	mFullscreen{ false }
,	mCurrentTime{ 0.0f }
,	mLastTime{ 0.0f }
,	mDeltaTime{ 0.0f }
,	mCursor{}
,	mCamera { nullptr }
,	mBaseProgram{ nullptr }
,	mLight{ nullptr }
,	mObjs{}
,	mQuadProgram{ nullptr }
,	mDepthProgram{ nullptr }
,	mFramebuffer{ nullptr }
,	mQuad{ nullptr }
{}


Window::~Window()
{}


void Window::initGlew()
{
	glewExperimental = GL_TRUE; // Initialize GLEW and handle error
	if (glewInit() != GLEW_OK)
	{
		throw GlewException(tag);
	}
}


void Window::handleInput(const input::Input i)
{
	for (auto& entity : mEntities)
	{
		entity->Handle(i);
	}
}


void Window::render()
{
	input::Input i{ pollInput() };
	render(computeDeltaTime(), i);
}


void Window::render(const float& deltaTime, const input::Input& input) // TODO comment
{
	// std::cout << static_cast<int>(1.0f / deltaTime) << " "; // FPS
	updateFrameSize();

	mCollision.Update();

	for (auto entity : mEntities)
	{
		entity->Update(deltaTime, input);
	}

	if (mStereoscopic)
	{
		renderStereoscopic(deltaTime);
	}
	else
	{
		render3D(deltaTime);
	}
}


void Window::renderGltf(const float& deltaTime)
{
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glViewport(0, 0, mFrameSize.width, mFrameSize.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
	mBaseProgram->use();
	mCamera->update(deltaTime, *mBaseProgram);
	mLight->Update(*mBaseProgram);

	mGltfRenderer->Draw(*mBaseProgram);
}


void Window::render3D(const float& deltaTime) // TODO comment
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, mFrameSize.width, mFrameSize.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
	mBaseProgram->use();
	mCamera->update(deltaTime, *mBaseProgram);
	mLight->Update(*mBaseProgram);
	for (WavefrontObject *obj : mObjs) { obj->draw(*mBaseProgram); }
	for (Entity* entity : mEntities)
	{
		if (entity->GetModel())
		{
			auto pModel = entity->GetModel();
			pModel->GetRenderer().Draw(*mBaseProgram, &pModel->GetNode());
		}
	}
	if (mGltfRenderer)
	{
		mGltfRenderer->Draw(*mBaseProgram);
	}
}


void Window::renderQuad(const float& /* deltaTime */)
{
	glViewport(0, 0, mFrameSize.width, mFrameSize.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color buffer
	mQuadProgram->use();
	mQuad->bind();
	mQuad->render();
	mQuad->unbind();
}

#ifdef SST_PROFILING
static int profiling_line{ 0 };
#endif

void Window::renderStereoscopic(const float& deltaTime)
{
#ifdef SST_PROFILING
	auto t1 = std::chrono::high_resolution_clock::now();
#endif

	// First pass
	glEnable(GL_DEPTH_TEST);
	mFramebuffer->bind(); // Render the scene on a framebuffer
	glViewport(0, 0, mFramebuffer->getWidth(), mFramebuffer->getHeight()); // Viewport for framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
	mBaseProgram->use();
	mCamera->update(deltaTime, *mBaseProgram);
	mLight->Update(*mBaseProgram);
	for (WavefrontObject *obj : mObjs) { obj->draw(*mBaseProgram); }
	mFramebuffer->unbind();
	// End First pass

#ifdef SST_PROFILING
	auto t2 = std::chrono::high_resolution_clock::now();
#endif

	// Second pass
	glDisable(GL_DEPTH_TEST);
	mQuad->bind();
	glViewport(0, 0, mFrameSize.width / 2, mFrameSize.height);
	mQuadProgram->use();
	mFramebuffer->bindColorTexture(*mQuadProgram); // Render color on the left
	mQuad->render();

#ifdef SST_PROFILING
	auto t3 = std::chrono::high_resolution_clock::now();
#endif

	glViewport(mFrameSize.width / 2, 0, mFrameSize.width / 2, mFrameSize.height);
	mDepthProgram->use();
	mFramebuffer->bindDepthTexture(*mDepthProgram); // Render depth on the right
	mQuad->render();
	mQuad->unbind();
	// End Second pass

#ifdef SST_PROFILING
	auto t4 = std::chrono::high_resolution_clock::now();

	float pass1 = (t2 - t1).count(); // Pass 1
	float pass2_1 = (t3 - t2).count(); // Pass 2.1
	float pass2_2 = (t4 - t3).count(); // Pass 2.2
	float pass2 = pass2_1 + pass2_2;
	float total = pass1 + pass2_1 + pass2_2;
	std::cout << ++profiling_line << "\t" // Frame number
		<< pass1 << "\t" // Pass 1
		<< pass2_1 << "\t" // Pass 2.1
		<< pass2_2 << "\t" // Pass 2.2
		<< total << "\t" // Total
		<< pass2 / total * 100.0f // Overhead
		<< std::endl;
#endif
}
