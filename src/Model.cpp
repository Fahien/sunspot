#include <iostream>

#include "Model.h"


Model::Model()
	: vbo_{0}
	, vao_{0}
{
	GLfloat vertices[] = {
		// Positions         // Colors
		 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, // Bottom left
		-1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, // Bottom right
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Top
	};
	glGenBuffers(1, &vbo_); // Generate the vertex buffer object
	glGenVertexArrays(1, &vao_); // Generate the vertex array object
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_); // Copy the vertices in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	// Specify the vertex shader input in the form of vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	std::cout << "Model -> created\n"; // TODO remove debug log
}


Model::~Model()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);

	std::cout << "Model -> destroyed\n"; // TODO remove debuf log
}
