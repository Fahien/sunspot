#ifndef SST_MATH_H
#define SST_MATH_H

#include <initializer_list>

namespace math {

class Vec3 {
	Vec3();
	Vec3(const float xx, const float yy, const float zz);

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

	float matrix_[16];
};

}

#endif // SST_MATH_H