#ifndef SST_GLTFCAMERA_H_
#define SST_GLTFCAMERA_H_

#include <gltfspot/Gltf.h>

#include "sunspot/component/Transform.h"
#include "sunspot/graphics/Shader.h"

namespace sunspot
{
class GltfCamera
{
  public:
	GltfCamera() {}
	static std::unique_ptr<GltfCamera> create( gltfspot::Gltf::Camera& c );

	void Translate( const mathspot::Vec3& t );
	void Rotate( const mathspot::Quat& q );

	void SetView( const mathspot::Mat4& v ) { m_View = v; }
	void Update( const graphics::shader::Program& );

  protected:
	GltfCamera( const float f, const float n )
	    : m_ZFar{ f }
	    , m_ZNear{ n }
	    , m_Rotation{ mathspot::Mat4::identity }
	    , m_Translation{ mathspot::Mat4::identity }
	    , m_View{ mathspot::Mat4::identity }
	    , m_Projection{ mathspot::Mat4::zero }
	{
	}

	void updateView() { m_View = m_Rotation * m_Translation; }

	float m_ZFar;
	float m_ZNear;

	mathspot::Mat4 m_Rotation;
	mathspot::Mat4 m_Translation;
	mathspot::Mat4 m_View;
	mathspot::Mat4 m_Projection;

	component::Transform m_Transform;
};


class GltfPerspectiveCamera : public GltfCamera
{
  public:
	GltfPerspectiveCamera( const float a = 1.0f, const float y = 0.7f, const float f = 100.0f, const float n = 0.0125f );

	static GltfCamera& Default();

	float m_AspectRatio;
	float m_YFov;
};


class GltfOrthographicCamera : public GltfCamera
{
  public:
	GltfOrthographicCamera( const float r, const float t, const float f, const float n )
	    : GltfCamera{ f, n }
	    , m_XMag{ r }
	    , m_YMag{ t }
	{
	}

	float m_XMag;
	float m_YMag;
};


}  // namespace sunspot


#endif  // SST_GLTFCAMERA_H_
