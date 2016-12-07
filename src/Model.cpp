#include <iostream>
#include <cmath>

#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"


using namespace sunspot;


Model::Model(const std::string &path, const float scale)
	: transform{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 }
	, vbo_ {0}
	, ebo_ {0}
	, vao_ {0}
	, material_{}
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

	TextureData diffuse{ path };
	glGenTextures(1, &material_.diffuseMap); // Create a diffuse map
	glBindTexture(GL_TEXTURE_2D, material_.diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, diffuse.getWidth(), diffuse.getHeight(),
		0, GL_RGB, GL_UNSIGNED_BYTE, diffuse.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	TextureData specular{ path + "-s" };
	glGenTextures(1, &material_.specularMap); // Create a specular map
	glBindTexture(GL_TEXTURE_2D, material_.specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, specular.getWidth(), specular.getHeight(),
		0, GL_RGB, GL_UNSIGNED_BYTE, specular.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::cout << "Model: created\n"; // TODO remove debug log
}


Model::~Model()
{
	glDeleteTextures(1, &material_.diffuseMap);
	glDeleteTextures(1, &material_.specularMap);
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);

	std::cout << "Model: destroyed\n"; // TODO remove debug log
}


void Model::render(const ShaderProgram *program) const
{
	// Bind transform matrix
	glUniformMatrix4fv(program->getLocation("model"), 1, GL_FALSE, transform.matrix);

	// Bind ambient color
	glUniform3f(program->getLocation("material.ambient"),
		material_.ambient.r, material_.ambient.g, material_.ambient.b);
	// Bind diffuse color
	glUniform3f(program->getLocation("material.diffuse"),
		material_.diffuse.r, material_.diffuse.g, material_.diffuse.b);
	// Bind specular color
	glUniform3f(program->getLocation("material.specular"),
		material_.specular.r, material_.specular.g, material_.specular.b);
	// Bind shininess
	glUniform1f(program->getLocation("material.shininess"), material_.shininess);
	// Bind diffuse map
	glUniform1i(program->getLocation("material.diffuseMap"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material_.diffuseMap);
	// Bind specular map
	glUniform1i(program->getLocation("material.specularMap"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material_.specularMap);

	// Eventually, draw
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

