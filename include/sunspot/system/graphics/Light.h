#pragma once

#include <mathspot/Math.h>

#include "sunspot/system/graphics/Shader.h"

#include "Color.h"

namespace mst = mathspot;

namespace sunspot::graphics
{


class Light
{
  public:
	Light( const float r = 1.0f, const float g = 1.0f, const float b = 1.0f );
	Light( const Color& color ) : m_Ambient { color }, m_Diffuse { color }, m_Specular {} {}

	const Color& GetAmbient()  const { return m_Ambient;  }
	const Color& GetDiffuse()  const { return m_Diffuse;  }
	const Color& GetSpecular() const { return m_Specular; }

	virtual void Update( const shader::Program& program ) const = 0;

  protected:
	Color m_Ambient  {};
	Color m_Diffuse  {};
	Color m_Specular {};
};


class DirectionalLight : public Light
{
  public:
	DirectionalLight( const Color& color ) : Light{ color } {}

	mst::Vec3& GetDirection() { return m_Direction; }
	void SetDirection( const float x, const float y, const float z ) { m_Direction.x = x; m_Direction.y = y; m_Direction.z = z; }

	void Update( const shader::Program& program ) const override;

  private:
	mst::Vec3 m_Direction {};
};


class PointLight : public Light
{
  public:
	PointLight( const Color& color ) : Light{ color } {}

	mst::Vec3& GetPosition() { return m_Position; }
	void SetPosition( const float x, const float y, const float z ) { m_Position.x = x; m_Position.y = y; m_Position.z = z; }

	void Update( const shader::Program& program ) const override;

  private:
	mst::Vec3 m_Position {};
};


} // namespace sunspot::graphics

