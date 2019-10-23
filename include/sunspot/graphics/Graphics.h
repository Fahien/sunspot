#pragma once

#include <stdexcept>
#include <unordered_set>

#include <mathspot/Math.h>

#include "sunspot/graphics/Light.h"
#include "sunspot/graphics/Shader.h"
#include "sunspot/graphics/gl/Renderer.h"

namespace sunspot::graphics
{
#ifdef ANDROID

class Graphics
{
  public:
	~Graphics();

	static Graphics& Get( ANativeWindow* window = nullptr );

	bool Swap();
	void DestroySurface();
	void Destroy();

  private:
	Graphics( ANativeWindow* window );

	void init( ANativeWindow* window );
	void initEGLSurface();
	void initEGLContext();

	ANativeWindow* mWindow;
	EGLDisplay     mDisplay;
	EGLSurface     mSurface;
	EGLConfig      mConfig;
	EGLContext     mContext;
};

#else

struct Viewport
{
	mathspot::Vec2 origin = {};
	mathspot::Size size   = {};
};

class Graphics
{
  public:
	void set_viewport( const Viewport& v ) { viewport = v; }

	void set_shader_program( const shader::Program* p ) { program = p; }

	void set_light( const Light* l ) { light = l; }

	/// @param g Gltf with the scene to draw
	void draw( gltfspot::Gltf& g );

  private:
	Viewport viewport = {};

	const shader::Program* program = {};

	const Light* light = {};

  public:
	gl::Renderer renderer = {};
};

#endif  // ANDROID


}  // namespace sunspot::graphics
