#include "Window.h"

#include <iostream>

#ifdef SST_PROFILING
#include <chrono>
#endif

#include "SunSpotConfig.h"
#include "sunspot/core/Game.h"
#include "view/GltfRenderer.h"
#include "view/GltfCamera.h"
#include "sunspot/system/graphic/Gl.h"
#include "sunspot/system/graphic/Shader.h"
#include "sunspot/system/graphic/Light.h"
#include "sunspot/system/graphic/Framebuffer.h"
#include "Camera.h"
#include "Quad.h"
#include "entity/Entity.h"
#include "component/Model.h"

using namespace mathspot;

namespace sunspot
{

const std::string Window::tag{ "Window" };


Window::Window( Game& game, const char* title, const mst::Size windowSize, const bool decorated, const bool stereoscopic)
:	m_Game { game }
,	m_Title       { title }
,	m_WindowSize  { windowSize }
,	m_MonitorSize { windowSize }
,	m_FrameSize   { windowSize }
,	mDecorated{ decorated }
,	mStereoscopic{ stereoscopic }
,	mFullscreen{ false }
,	mCurrentTime{ 0.0f }
,	mLastTime{ 0.0f }
,	mDeltaTime{ 0.0f }
,	mCursor{}
,	m_pCamera { nullptr }
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


void Window::handleInput( input::Input&& i )
{
	m_Game.Handle( std::move( i ) );
/*
	if ( i.action == input::Action::PRESS )
	{
		if ( i.key == input::Key::A )
		{
			m_Camera->GetTransform()->position.x += -1.0;
		}
		if ( i.key == input::Key::S )
		{
			m_Camera->GetTransform()->position.z += -1.0;
		}
		if ( i.key == input::Key::W )
		{
			m_Camera->GetTransform()->position.z += 1.0;
		}
		if ( i.key == input::Key::D )
		{
			m_Camera->GetTransform()->position.x += 1.0;
		}
		if ( i.key == input::Key::Q )
		{
			m_Camera->GetTransform()->rotation.x += 0.25;
			logspot::Logger::log.Info( "Camera Rotation X: %f", m_Camera->GetTransform()->rotation.x );
		}
		if ( i.key == input::Key::E )
		{
			m_Camera->GetTransform()->rotation.x += -0.25;
			logspot::Logger::log.Info( "Camera Rotation X: %f", m_Camera->GetTransform()->rotation.x );
		}
	}

	for ( auto& entity : mEntities )
	{
		entity->Handle( i );
	}
*/
}


void Window::render()
{
	render( 0.125f );
}


void Window::render(const float& deltaTime) // TODO comment
{
	// std::cout << static_cast<int>(1.0f / deltaTime) << " "; // FPS
	//updateFrameSize();

	mCollision.Update();

	for (auto entity : mEntities)
	{
		entity->Update(deltaTime);
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
	glViewport(0, 0, m_FrameSize.width, m_FrameSize.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
//	mBaseProgram->use();
	//mCamera->update(deltaTime, *mBaseProgram);
//	mLight->Update(*mBaseProgram);

//	mGltfRenderer->Draw(*mBaseProgram);
}


void Window::render3D( const float& deltaTime ) // TODO comment
{
	glEnable( GL_DEPTH_TEST );
	glViewport( 0, 0, m_FrameSize.width, m_FrameSize.height );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear color and depth buffer
	mBaseProgram->Use();
	mLight->Update( *mBaseProgram );

	m_Camera->Get<component::Camera>()->get().Update( *mBaseProgram );

	for ( auto pEntity : mEntities )
	{
		if ( pEntity->Has<component::Model>() )
		{
			auto& model = pEntity->Get<component::Model>()->get();
			model.GetRenderer().Draw( *mBaseProgram, &model.GetNode() );
		}
	}

	if ( mGltfRenderer )
	{
		mGltfRenderer->Draw( *mBaseProgram );
	}
}


void Window::renderQuad( const float& /* deltaTime */ )
{
	glViewport( 0, 0, m_FrameSize.width, m_FrameSize.height );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear color buffer
	mQuadProgram->Use();
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
	mBaseProgram->Use();
	m_pCamera->Update(*mBaseProgram);
	mLight->Update(*mBaseProgram);
//	for (WavefrontObject *obj : mObjs) { obj->draw(*mBaseProgram); }
	mFramebuffer->unbind();
	// End First pass

#ifdef SST_PROFILING
	auto t2 = std::chrono::high_resolution_clock::now();
#endif

	// Second pass
	glDisable(GL_DEPTH_TEST);
	mQuad->bind();
	glViewport(0, 0, m_FrameSize.width / 2, m_FrameSize.height);
	mQuadProgram->Use();
	mFramebuffer->bindColorTexture(*mQuadProgram); // Render color on the left
	mQuad->render();

#ifdef SST_PROFILING
	auto t3 = std::chrono::high_resolution_clock::now();
#endif

	glViewport(m_FrameSize.width / 2, 0, m_FrameSize.width / 2, m_FrameSize.height);
	mDepthProgram->Use();
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

} // namespace sunspot