#ifndef SST_GLTFCAMERA_H_
#define SST_GLTFCAMERA_H_

#include <mathspot/Math.h>

#include "sunspot/component/Transform.h"
#include "sunspot/graphics/Shader.h"

namespace mst = mathspot;


namespace sunspot
{

class GltfCamera
{
  public:
	GltfCamera(){}

	void Translate( const mst::Vec3& t );
	void Rotate( const mst::Quat& q );

	void SetView( const mst::Mat4& v ) { m_View = v; }
	void Update( const graphics::shader::Program& );

  protected:
	GltfCamera(const float f, const float n)
	:	m_ZFar { f }
	,	m_ZNear{ n }
	,	m_Rotation   { mst::Mat4::identity }
	,	m_Translation{ mst::Mat4::identity }
	,	m_View       { mst::Mat4::identity }
	,	m_Projection { mst::Mat4::zero     }
	{}

	void updateView() { m_View = m_Rotation * m_Translation; }

	float m_ZFar;
	float m_ZNear;

	mst::Mat4 m_Rotation;
	mst::Mat4 m_Translation;
	mst::Mat4 m_View;
	mst::Mat4 m_Projection;

	component::Transform m_Transform;
};


class GltfPerspectiveCamera : public GltfCamera
{
  public:
	GltfPerspectiveCamera(const float a = 1.0f, const float y = 0.7f, const float f = 100.0f, const float n = 0.0125f);

	static GltfCamera& Default();

	float m_AspectRatio;
	float m_YFov;
};


class GltfOrthographicCamera : public GltfCamera
{
  public:
	GltfOrthographicCamera(const float r, const float t, const float f, const float n)
	:	GltfCamera{ f , n }
	,	m_XMag{ r }
	,	m_YMag{ t }
	{}

	float m_XMag;
	float m_YMag;

};


}


#endif // SST_GLTFCAMERA_H_
