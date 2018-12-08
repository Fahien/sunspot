#ifndef SST_CAMERACOMPONENT_H_
#define SST_CAMERACOMPONENT_H_

#include <mathspot/Math.h>

#include "sunspot/entity/Object.h"
#include "sunspot/component/Transform.h"

namespace mst = mathspot;


namespace sunspot
{

class ShaderProgram;

namespace component
{


class Camera : public Object
{
  public:
	static constexpr float kYFov  { 0.7f   }; // 40 degrees
	static constexpr float kNear  { 0.125f };
	static constexpr float kFar   { 100.0f };

	Camera( const float f = kFar, const float n = kNear )
	:	m_ZFar  { f }
	,	m_ZNear { n }
	,	m_Rotation    { mst::Mat4::identity }
	,	m_Translation { mst::Mat4::identity }
	,	m_View        { mst::Mat4::identity }
	,	m_Projection  { mst::Mat4::zero     }
	{}
	virtual ~Camera() {}

	component::Transform& GetTransform();

	void Translate( const mst::Vec3& t );
	void Rotate( const mst::Quat& q );

	void SetView( const mst::Mat4& v ) { m_View = v; }
	void Update( const ShaderProgram& );

  protected:

	void updateView();

	float m_ZFar;
	float m_ZNear;

	mst::Mat4 m_Rotation;
	mst::Mat4 m_Translation;
	mst::Mat4 m_View;
	mst::Mat4 m_Projection;
};


class PerspectiveCamera : public Camera
{
  public:
	PerspectiveCamera( const float a = 1.0f, const float y = kYFov, const float f = kFar, const float n = kNear );

	static Camera& Default();

	void SetAspectRatio( const float a );

	float m_AspectRatio;
	float m_YFov;
};


class OrthographicCamera : public Camera
{
  public:
	OrthographicCamera( const float r, const float t, const float f, const float n )
	:	Camera { f , n }
	,	m_XMag { r }
	,	m_YMag { t }
	{}

	float m_XMag;
	float m_YMag;

};


} // namespace component

} // namespace sunspot


#endif // SST_CAMERACOMPONENT_H_
