#include "sunspot/component/Camera.h"

#include <cmath>
#include <mathspot/Math.h>

#include "sunspot/entity/Entity.h"

using namespace mathspot;

namespace sunspot::component
{


Camera& PerspectiveCamera::Default()
{
	static PerspectiveCamera camera;
	return camera;
}


PerspectiveCamera::PerspectiveCamera( const float a, const float y, const float f, const float n )
:	Camera        { f , n }
,	m_AspectRatio { a }
,	m_YFov        { y }
{
	// Calculate projection matrix
	float cotfov { 1.0f / std::tan( 0.5f * y ) };
	m_Projection[0]  = cotfov / a;
	m_Projection[5]  = cotfov;
	m_Projection[10] = -( n + f ) / ( f - n );
	m_Projection[14] = -2.0f * n * f / ( f - n );
	m_Projection[11] = -1.0f;

	// Calculate view matrix
	auto direction = Vec3 { 0.0f, 0.0f, 1.0f };
	auto right     = Vec3::Cross( direction, Vec3{ 0.0f, 1.0f, 0.0f } );
	auto up        = Vec3::Cross( right, direction );
	up.Normalize();

	m_Rotation[0]  = right.x;
	m_Rotation[1]  = up.x;
	m_Rotation[2]  = direction.x;
	m_Rotation[4]  = right.y;
	m_Rotation[5]  = up.y;
	m_Rotation[6]  = direction.y;
	m_Rotation[8]  = right.z;
	m_Rotation[9]  = up.z;
	m_Rotation[10] = direction.z;
}

void PerspectiveCamera::SetAspectRatio( const float a )
{
	const float cotfov { 1.0f / std::tan( 0.5f * m_YFov ) };
	m_Projection[0] = cotfov / a;
	m_AspectRatio = a;
}

constexpr float OrthographicCamera::kRight  = 20.0f;
constexpr float OrthographicCamera::kLeft   = -20.0f;
constexpr float OrthographicCamera::kTop    = 20.0f;
constexpr float OrthographicCamera::kBottom = -20.0f;
constexpr float OrthographicCamera::kNear   = 0.0f;
constexpr float OrthographicCamera::kFar    = 20.0f;

OrthographicCamera::OrthographicCamera( const float r, const float l, const float t, const float b, const float f, const float n )
:	Camera   { f , n }
,	m_Right  { r }
,	m_Left   { l }
,	m_Top    { t }
,	m_Bottom { b }
{
	// Calculate projection matrix
	m_Projection[0]  = 2.0f / ( r - l );
	m_Projection[5]  = 2.0f / ( t - b );
	m_Projection[10] = -2.0f / ( f - n );
	m_Projection[12] = - ( r + l ) / ( r - l );
	m_Projection[13] = - ( t + b) / ( t - b );
	m_Projection[14] = -( f + n ) / ( f - n );
	m_Projection[15] = 1.0f;

	// Calculate view matrix
	auto direction = Vec3 { 0.0f, 0.0f, 1.0f };
	auto right     = Vec3 { 1.0f, 0.0f, 0.0f };
	auto up        = Vec3 { 0.0f, 1.0f, 0.0f };

	m_Rotation[0]  = right.x;
	m_Rotation[1]  = up.x;
	m_Rotation[2]  = direction.x;
	m_Rotation[4]  = right.y;
	m_Rotation[5]  = up.y;
	m_Rotation[6]  = direction.y;
	m_Rotation[8]  = right.z;
	m_Rotation[9]  = up.z;
	m_Rotation[10] = direction.z;
}

component::Transform& Camera::GetTransform()
{
	auto pParent = Object::GetParent();
	assert( pParent && "Camera component is not attached to an entity" );

	auto pTransform = pParent->GetTransform();
	assert( pTransform && "Camera entity has no transform" );

	return *pTransform;
}


void Camera::Translate( const Vec3& t )
{
	m_Translation.Translate( -t );

	GetTransform().position.x = -m_Translation[12];
	GetTransform().position.y = -m_Translation[13];
	GetTransform().position.z = -m_Translation[14];

	updateView();
}


void Camera::Rotate( const Quat& q )
{
	m_Rotation.Rotate( q );

	updateView();
}

void Camera::updateView()
{
	auto& transform = GetTransform();
	m_Translation[12] = transform.position.x;
	m_Translation[13] = transform.position.y;
	m_Translation[14] = transform.position.z;

	m_Rotation = mst::Mat4::identity;
	m_Rotation.RotateX( transform.rotation.x );
	m_Rotation.RotateY( transform.rotation.y );
	m_Rotation.RotateZ( transform.rotation.z );

	m_View = m_Rotation * m_Translation;
}

void Camera::Update( const graphic::shader::Program& program )
{
	// View
	updateView();
	auto location = program.GetLocation( "view" );
	glUniformMatrix4fv( location, 1, GL_FALSE, m_View.matrix );

	// Projection
	location = program.GetLocation( "projection" );
	glUniformMatrix4fv( location, 1, GL_FALSE, m_Projection.matrix );

	// Position
	location = program.GetLocation( "camera.position" );

	auto& position = GetTransform().position;
	glUniform3fv( location, 1, &position.x );
}


} // namespace sunspot::component