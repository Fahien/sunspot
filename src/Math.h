#ifndef SST_MATH_H
#define SST_MATH_H

#include <initializer_list>
#include <iostream>


namespace math {


const float pi{ 3.14159265f };


class Size {
public:
	Size();
	Size(int w, int h);

	Size &operator*=(const int &f);
	Size &operator*=(const float &f);
	Size &operator/=(const int &i);
	const Size operator/(const int &i) const;

	friend std::ostream &operator<<(std::ostream &os, const Size &v);

	int width;
	int height;
};


class Vec2 {
public:
	Vec2();
	Vec2(const float xx, const float yy);

	void normalize();
	Vec2 &operator=(const Vec2 &other);
	Vec2 &operator+=(const Vec2 &other);
	const Vec2 operator+(const Vec2 &other) const;

	friend std::ostream &operator<<(std::ostream &os, const Vec2 &v);

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

std::ostream &operator<<(std::ostream &os, const Vec3 &v);


class Mat4 {
public:
	Mat4();
	Mat4(std::initializer_list<float>);
	Mat4(float *m);

	static Mat4 identity;

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


class Rectangle
{
public:
	Rectangle();

	/// Tests whether (x, y) is inside the rectangle
	bool contains(float x, float y);

	/// Tests whether this box intersects another rectangle
	bool intersects(const Rectangle &box);
	/// Tests whether this box intersects another rectangle
	bool intersects(const Rectangle *box);

	float x;
	float y;
	float width;
	float height;
};

}


#endif // SST_MATH_H
