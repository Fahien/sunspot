#include "Math.h"

using namespace math;

Mat4::Mat4()
	: matrix_{ 0 }
{}


Mat4::Mat4(std::initializer_list<float> list)
{
	int i{ 0 };
	for (float value : list) {
		matrix_[i] = value;
		++i;
		if (i == 16) { break; }
	}
}


Mat4::Mat4(float *matrix)
{
	for (int i{ 0 }; i < 16; ++i) {
		matrix_[i] = matrix[i];
	}
}

float &Mat4::operator[](const int index)
{
	return matrix_[index];
}

float Mat4::operator[](const int index) const
{
	return matrix_[index];
}

Mat4 &Mat4::operator=(const Mat4 &matrix)
{
	for (int i{ 0 }; i < 16; ++i) {
		matrix_[i] = matrix.matrix_[i];
	}
	return *this;
}


Mat4 &Mat4::operator+=(const Mat4 &matrix)
{
	for (int i{ 0 }; i < 16; ++i) {
		matrix_[i] += matrix.matrix_[i];
	}
	return *this;
}


const Mat4 Mat4::operator+(const Mat4 &other) const
{
	Mat4 result = *this;
	return result += other;
}


Mat4 &Mat4::operator*=(const Mat4 &matrix)
{
	float temp[16];
	for (int i{ 0 }; i < 4; ++i) {
		for (int j{ 0 }; j < 4; ++j) {
			temp[i+j*4] = matrix_[i] * matrix[j*4] + matrix_[i+4] * matrix[j*4+1] + matrix_[i+8] * matrix[j*4+2] + matrix_[i+12] * matrix[j*4+3];
		}
	}
	for (int i{ 0 }; i < 16; ++i) {
		matrix_[i] = temp[i];
	}
	return *this;
}


const Mat4 Mat4::operator*(const Mat4 &other) const
{
	Mat4 result = *this;
	return result *= other;
}