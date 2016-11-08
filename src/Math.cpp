#include <cmath>

#include "Math.h"

using namespace math;


Vec2::Vec2()
	: x{ 0.0f }
	, y{ 0.0f }
{}


Vec2::Vec2(const float xx, const float yy)
	: x{ xx }
	, y{ yy }
{}


void Vec2::normalize()
{
	float length{ sqrt(x * x + y * y) };
	x /= length;
	y /= length;
}


Vec2 &Vec2::operator=(const Vec2 &other)
{
	x = other.x;
	y = other.y;
	return *this;
}


Vec2 &Vec2::operator+=(const Vec2 &other)
{
	x += other.x;
	y += other.y;
	return *this;
}


const Vec2 Vec2::operator+(const Vec2 &other) const
{
	Vec2 result = *this;
	return result += other;
}


Vec3::Vec3()
	: x{ 0.0f }
	, y{ 0.0f }
	, z{ 0.0f }
{}


Vec3::Vec3(const float xx, const float yy, const float zz)
	: x {xx}
	, y {yy}
	, z {zz}
{}


Vec3 &Vec3::cross(const Vec3 &a, const Vec3 &b)
{
	Vec3 result{};
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return result;
}


void Vec3::normalize()
{
	float length{ sqrtf(x * x + y * y + z * z) };
	x /= length;
	y /= length;
	z /= length;
}


Vec3 &Vec3::operator=(const Vec3 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}


Vec3 &Vec3::operator+=(const Vec3 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}


Mat4::Mat4()
	: matrix{ 0.0f }
{}


Mat4::Mat4(std::initializer_list<float> list)
{
	int i{ 0 };
	for (float value : list) {
		matrix[i] = value;
		++i;
		if (i == 16) { break; }
	}
}


Mat4::Mat4(float *matrix)
{
	for (int i{ 0 }; i < 16; ++i) {
		matrix[i] = matrix[i];
	}
}


float &Mat4::operator[](const int index)
{
	return matrix[index];
}


float Mat4::operator[](const int index) const
{
	return matrix[index];
}


Mat4 &Mat4::operator=(const Mat4 &other)
{
	for (int i{ 0 }; i < 16; ++i) {
		matrix[i] = other.matrix[i];
	}
	return *this;
}


Mat4 &Mat4::operator+=(const Mat4 &other)
{
	for (int i{ 0 }; i < 16; ++i) {
		matrix[i] += other.matrix[i];
	}
	return *this;
}


const Mat4 Mat4::operator+(const Mat4 &other) const
{
	Mat4 result = *this;
	return result += other;
}


Mat4 &Mat4::operator*=(const Mat4 &other)
{
	float temp[16];
	for (int i{ 0 }; i < 4; ++i) {
		for (int j{ 0 }; j < 4; ++j) {
			temp[i+j*4] = matrix[i] * other[j*4] + matrix[i+4] * other[j*4+1] + matrix[i+8] * other[j*4+2] + matrix[i+12] * other[j*4+3];
		}
	}
	for (int i{ 0 }; i < 16; ++i) {
		matrix[i] = temp[i];
	}
	return *this;
}


const Mat4 Mat4::operator*(const Mat4 &other) const
{
	Mat4 result = *this;
	return result *= other;
}



void Mat4::translateX(const float amount)
{
	matrix[12] += amount;
}


void Mat4::translateY(const float amount)
{
	matrix[13] += amount;
}


void Mat4::translateZ(const float amount)
{
	matrix[14] += amount;
}


void Mat4::rotateX(const float radians)
{
	float cosrad{ cos(radians) };
	float sinrad{ sin(radians) };
	math::Mat4 rotation{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosrad, sinrad, 0.0f,
		0.0f, -sinrad, cosrad, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	*this = rotation * *this;
}


void Mat4::rotateY(const float radians)
{
	float cosrad{ cos(radians) };
	float sinrad{ sin(radians) };
	math::Mat4 rotation{
		cosrad, 0.0f, -sinrad, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinrad, 0.0f, cosrad, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	*this = rotation * *this;
}


void Mat4::rotateZ(const float radians)
{
	float cosrad{ cos(radians) };
	float sinrad{ sin(radians) };
	math::Mat4 rotation{
		cosrad, sinrad, 0, 0,
		-sinrad, cosrad, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	*this = rotation * *this;
}