#ifndef SST_MATH_H
#define SST_MATH_H

#include <initializer_list>

namespace math {


const float pi{ 3.14159265f };


class Vec2 {
  public:
	Vec2();
	Vec2(const float xx, const float yy);

	void normalize();
	Vec2 &operator=(const Vec2 &other);
	Vec2 &operator+=(const Vec2 &other);
	const Vec2 operator+(const Vec2 &other) const;

	float x;
	float y;
};


class Vec3 {
  public:
	Vec3();
	Vec3(const float xx, const float yy, const float zz);

	static Vec3 cross(const Vec3 &a, const Vec3 &b);

	void normalize();
	Vec3 &operator=(const Vec3 &other);
	Vec3 &operator+=(const Vec3 &other);

	float x;
	float y;
	float z;
};


class Mat4 {
  public:
	Mat4();
	Mat4(std::initializer_list<float>);
	Mat4(float *matrix);

	float &operator[](const int index);
	float operator[](const int index) const;
	Mat4 &operator=(const Mat4 &matrix);
	Mat4 &operator+=(const Mat4 &matrix);
	const Mat4 operator+(const Mat4 &other) const;
	Mat4 &operator*=(const Mat4 &matrix);
	const Mat4 operator*(const Mat4 &other) const;

	void translateX(const float amount);
	void translateY(const float amount);
	void translateZ(const float amount);
	void rotateX(const float radians);
	void rotateY(const float radians);
	void rotateZ(const float radians);

	float matrix[16];
};

}

#endif // SST_MATH_H