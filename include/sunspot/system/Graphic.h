#pragma once

#include <stdexcept>
#include <unordered_set>

#include <mathspot/Math.h>

#include "sunspot/system/graphic/Shader.h"
#include "sunspot/system/graphic/Light.h"
#include "sunspot/entity/Entity.h"
#include "sunspot/component/Model.h"

namespace sunspot
{

namespace graphic
{

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

#else

class System
{
  public:
	struct Viewport
	{
		mathspot::Vec2 origin = {};
		mathspot::Size size   = {};
	};

	void SetViewport( const Viewport& v ) { m_Viewport = v; }
	void SetShaderProgram( const shader::Program* pProgram ) { m_pProgram = pProgram; }
	void SetLight( const Light* pLight ) { m_pLight = pLight; }
	void SetCamera( Entity& camera ) { m_pCamera = &camera; }
	void AddModel( component::Model* pModel ) { m_Models.emplace( pModel ); }

	void Draw();

  private:
	Viewport m_Viewport = {};

	const shader::Program* m_pProgram = {};

	const Light* m_pLight = {};

	Entity* m_pCamera = {};

	std::unordered_set<component::Model*> m_Models = {};
};

#endif // ANDROID


} // namespace system

} // namespace sunspot
