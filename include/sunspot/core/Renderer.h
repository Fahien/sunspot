#ifndef SST_RENDERER_H_
#define SST_RENDERER_H_

#include <EGL/egl.h>
#include "core/ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "sunspot/entity/Entity.h"

namespace sunspot
{

class GltfRenderer;


class Renderer
{
  public:
	Renderer();
	virtual ~Renderer();

	static Renderer* createRenderer();

	bool init();
	void resize(int w, int h);
	void render();

  private:
	void step();
	virtual void draw();
	bool checkGlError(const char* funcName);

	uint64_t mLastFrameNs;

	const EGLContext mEglContext;

	ShaderProgram mShaderProgram;
	Camera mCamera;
	PointLight mLight;

	std::vector<Entity*> mEntities;
	GltfRenderer* mGltfRenderer { nullptr };
};


}


#endif // SST_RENDERER_H_