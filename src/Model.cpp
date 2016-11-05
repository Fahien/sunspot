#include <iostream>
#include <cmath>

#include "Model.h"


Model::Model()
	: vbo_ {0}
	, ebo_ {0}
	, vao_ {0}
	, transform_ {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1}
{
	GLfloat vertices[] = {
		// Positions         // Colors
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom left
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom right
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Top right
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, // Top left
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Bottom left rear
		 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Bottom right rear
		 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Top right rear
		-0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, // Top left rear
	};
	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3,
		1, 5, 6,
		1, 6, 2,
		5, 4, 7,
		5, 7, 6,
		4, 0, 3,
		4, 3, 7,
		1, 0, 4,
		1, 4, 5,
		3, 2, 6,
		3, 6, 7
	};
	glGenBuffers(1, &vbo_); // Generate the vertex buffer object
	glGenBuffers(1, &ebo_); // Generate the element buffer object
	glGenVertexArrays(1, &vao_); // Generate the vertex array object
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_); // Copy the vertices in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_); // Copy the indices in the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
	// Specify the vertex shader input in the form of vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // Unbind vao

	std::cout << "Model -> created\n"; // TODO remove debug log
}


Model::~Model()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);

	std::cout << "Model -> destroyed\n"; // TODO remove debuf log
}


void Model::translateX(const float amount)
{
	transform_[12] += amount;
}


void Model::rotateY(const float radians)
{
	float cosrad{ cos(radians) };
	float sinrad{ sin(radians) };
	math::Mat4 rotation{
		cosrad, 0.0f, -sinrad, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinrad, 0.0f, cosrad, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	transform_ = rotation * transform_;
	/*
	float t00 {transform_[0][0] * cosrad + transform_[2][0] * -sinrad};
	float t20 {transform_[0][0] * sinrad + transform_[2][0] * cosrad};
	float t02 {transform_[0][2] * cosrad + transform_[2][2] * -sinrad};
	float t22 {transform_[0][2] * sinrad + transform_[2][2] * cosrad};
	transform_[0][0] = t00;
	transform_[2][0] = t20;
	transform_[0][2] = t02;
	transform_[2][2] = t22;
	*/
}


void Model::rotateZ(const float radians)
{
	float cosrad{ static_cast<float>(cos(radians)) };
	float sinrad{ static_cast<float>(sin(radians)) };
	math::Mat4 rotation{
		cosrad, sinrad, 0, 0,
		-sinrad, cosrad, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	transform_ = rotation * transform_;

	/*
	float t00 {transform_[0][0] * cosrad + transform_[1][0] * sinrad};
	float t10 {transform_[0][0] * -sinrad + transform_[1][0] * cosrad};
	float t01 {transform_[0][1] * cosrad + transform_[1][1] * sinrad};
	float t11 {transform_[0][1] * -sinrad + transform_[1][1] * cosrad};
	transform_[0][0] = t00;
	transform_[0][1] = t01;
	transform_[1][0] = t10;
	transform_[1][1] = t11;
	*/
}
