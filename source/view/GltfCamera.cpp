#include "view/GltfCamera.h"

#include <cmath>

using namespace sunspot;
using namespace mathspot;


GltfCamera& GltfPerspectiveCamera::Default()
{
	static GltfPerspectiveCamera camera;
	camera.Translate( Vec3{ 0.0f, 0.0f, 3.0f } );
	return camera;
}

std::unique_ptr<GltfCamera> GltfCamera::create( gltfspot::Camera& c )
{
	if ( c.type == gltfspot::Camera::Type::Perspective )
	{
		return std::make_unique<GltfPerspectiveCamera>( c.perspective.aspect_ratio, c.perspective.yfov, c.perspective.zfar,
		                                                c.perspective.znear );
	}

	return std::make_unique<GltfOrthographicCamera>( c.orthographic.xmag, c.orthographic.ymag, c.orthographic.zfar,
	                                                 c.orthographic.znear );
}

GltfPerspectiveCamera::GltfPerspectiveCamera( const float a, const float y, const float f, const float n )
    : GltfCamera{ f, n }
    , m_AspectRatio{ a }
    , m_YFov{ y }
{
	// Calculate projection matrix
	float cotfov{ 1.0f / std::tan( 0.5f * y ) };
	m_Projection[0]  = cotfov / a;
	m_Projection[5]  = cotfov;
	m_Projection[10] = -( n + f ) / ( f - n );
	m_Projection[14] = -2.0f * n * f / ( f - n );
	m_Projection[11] = -1.0f;

	// Calculate view matrix
	auto direction = Vec3{ 0.0f, 0.0f, 1.0f };
	auto right     = Vec3::Cross( direction, Vec3{ 0.0f, 1.0f, 0.0f } );
	auto up        = Vec3::Cross( right, direction );
	up.Normalize();

	m_Rotation[0]     = right.x;
	m_Rotation[1]     = up.x;
	m_Rotation[2]     = direction.x;
	m_Rotation[4]     = right.y;
	m_Rotation[5]     = up.y;
	m_Rotation[6]     = direction.y;
	m_Rotation[8]     = right.z;
	m_Rotation[9]     = up.z;
	m_Rotation[10]    = direction.z;
	m_Translation[12] = -m_Transform.position.x;
	m_Translation[13] = -m_Transform.position.y;
	m_Translation[14] = -m_Transform.position.z;

	updateView();
}


void GltfCamera::Translate( const Vec3& t )
{
	m_Translation.Translate( -t );

	m_Transform.position.x = -m_Translation[12];
	m_Transform.position.y = -m_Translation[13];
	m_Transform.position.z = -m_Translation[14];

	updateView();
}


void GltfCamera::Rotate( const Quat& q )
{
	m_Rotation.Rotate( q );

	updateView();
}


void GltfCamera::Update( const graphics::shader::Program& program )
{
	auto location = program.get_location( "view" );
	glUniformMatrix4fv( location, 1, GL_FALSE, m_View.matrix );

	location = program.get_location( "projection" );
	glUniformMatrix4fv( location, 1, GL_FALSE, m_Projection.matrix );

	location = program.get_location( "camera.position" );
	glUniform3fv( location, 1, &m_View.matrix[12] );
}
