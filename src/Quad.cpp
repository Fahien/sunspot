#include "Quad.h"

using namespace sunspot;


const Logger Quad::log{};


Quad::Quad()
	: vbo_{ 0 }
	, ebo_{ 0 }
	, vao_{ 0 }
{
	GLfloat vertices[] = {
		// Positions  // TexCoords
		-1.0f, -1.0f, 0.0, 0.0, // Bottom left
		 1.0f, -1.0f, 1.0, 0.0, // Bottom right
		 1.0f,  1.0f, 1.0, 1.0, // Top right
		 -1.0f, 1.0f, 0.0, 1.0  // Top left
	};
	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // Unbind vao

	log.info("Quad: created\n"); // TODO remove debug log
}


Quad::~Quad()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);

	log.info("Quad: destroyed\n"); // TODO remove debuf log
}
