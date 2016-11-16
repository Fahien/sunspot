#include <iostream>
#include <cmath>

#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"


Model::Model(const char *path, const float scale)
	: transform{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 }
	, vbo_ {0}
	, ebo_ {0}
	, vao_ {0}
	, material_{}
	, texture_ {0}
{
	GLfloat vertices[] = {
		// Positions                                 //Normal           // TexCoords
		-1.0f * scale, -1.0f * scale, -1.0f * scale, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom left Front
		1.0f * scale, -1.0f * scale, -1.0f * scale,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // Bottom right
		1.0f * scale,  1.0f * scale, -1.0f * scale,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // Top right
		-1.0f * scale,  1.0f * scale, -1.0f * scale, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // Top left

		1.0f * scale, -1.0f * scale, -1.0f * scale,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom left Right
		1.0f * scale, -1.0f * scale, 1.0f * scale,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // Bottom right
		1.0f * scale, 1.0f * scale, 1.0f * scale,    1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // Top right
		1.0f * scale, 1.0f * scale, -1.0f * scale,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // Top left

		-1.0f * scale, -1.0f * scale, 1.0f * scale,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Bottom left Rear
		1.0f * scale, -1.0f * scale, 1.0f * scale,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom right
		1.0f * scale,  1.0f * scale, 1.0f * scale,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // Top right rear
		-1.0f * scale,  1.0f * scale, 1.0f * scale,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top left rear

		-1.0f * scale, -1.0f * scale, -1.0f * scale, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom left Left
		-1.0f * scale, -1.0f * scale, 1.0f * scale,  -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom right
		-1.0f * scale, 1.0f * scale, 1.0f * scale,   -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top right
		-1.0f * scale, 1.0f * scale, -1.0f * scale,  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top left

		-1.0f * scale, 1.0f * scale, -1.0f * scale,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // Bottom left Top
		1.0f * scale, 1.0f * scale, -1.0f * scale,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom right
		1.0f * scale, 1.0f * scale, 1.0f * scale,    0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // Top right
		-1.0f * scale, 1.0f * scale, 1.0f * scale,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top left

		-1.0f * scale, -1.0f * scale, -1.0f * scale, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom left Bottom
		1.0f * scale, -1.0f * scale, -1.0f * scale,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Bottom right
		1.0f * scale, -1.0f * scale, 1.0f * scale,   0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // Top right
		-1.0f * scale, -1.0f * scale, 1.0f * scale,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Top left
	};
	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind vao

	Texture texture{ path };
	glGenTextures(1, &texture_); // Create a texture for colors
	glBindTexture(GL_TEXTURE_2D, texture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.getData()); // TODO remove magic numbers
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::cout << "Model: created\n"; // TODO remove debug log
}


Model::~Model()
{
	glDeleteTextures(1, &texture_);
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);

	std::cout << "Model: destroyed\n"; // TODO remove debug log
}


void Model::render(const ShaderProgram *program) const
{
	GLuint location{ program->getLocation("model") };
	glUniformMatrix4fv(location, 1, GL_FALSE, transform.matrix);

	location = program->getLocation("material.ambient");
	glUniform3fv(location, 1, &material_.ambient.r);
	location = program->getLocation("material.diffuse");
	glUniform3fv(location, 1, &material_.diffuse.r);
	location = program->getLocation("material.specular");
	glUniform3fv(location, 1, &material_.specular.r);
	location = program->getLocation("material.shininess");
	glUniform1f(location, material_.shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}