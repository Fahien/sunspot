#ifndef SST_RENDERER_H_
#define SST_RENDERER_H_

#include <EGL/egl.h>
#include "core/ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "sunspot/entity/Entity.h"
#include <dataspot/DataSpot.h>
#include "repository/ModelRepository.h"
#include "repository/EntityRepository.h"
#include "system/Collision.h"

namespace sunspot
{

class GltfRenderer;


class Renderer
{
  public:
	Renderer(const std::string& db);
	virtual ~Renderer();

	static Renderer* New(const std::string& db);

	void Add(Entity* pEntity);

	bool init();
	void resize(int w, int h);
	void render();

  private:
	void step();
	virtual void draw();
	bool checkGlError(const char* funcName);

	uint64_t mLastFrameNs{ 0 };
	float mDelta{ 0.0f };

	const EGLContext mEglContext;

	ShaderProgram mShaderProgram;
	Camera mCamera;
	PointLight mLight;

	std::vector<Entity*> mEntities;
	GltfRenderer* mGltfRenderer { nullptr };

	dst::DataSpot mDataSpot;
	ModelRepository mModelRepository;
	EntityRepository mEntityRepository;

	system::Collision mCollision;
};


}


#endif // SST_RENDERER_H_