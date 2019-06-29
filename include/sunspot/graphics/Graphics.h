#ifndef SUNSPOT_GRAPHICS_H_
#define SUNSPOT_GRAPHICS_H_

#include <stdexcept>
#include <unordered_set>

#include <mathspot/Math.h>

#include "sunspot/component/Model.h"
#include "sunspot/entity/Entity.h"
#include "sunspot/graphics/Light.h"
#include "sunspot/graphics/Shader.h"

namespace sunspot
{
namespace graphics
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
	void set_camera( Entity& c ) { camera = &c; }
	void add_model( component::Model* model ) { models.emplace( model ); }

	void draw();

  private:
	Viewport viewport = {};

	const shader::Program* program = {};

	const Light* light = {};

	Entity* camera = {};

	std::unordered_set<component::Model*> models = {};
};

#endif  // ANDROID


}  // namespace graphics
}  // namespace sunspot

#endif  // SUNSPOT_GRAPHICS_H_
