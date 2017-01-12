#include <iostream>
#include <string>
#include "Mesh.h"
#include "ShaderProgram.h"

using namespace sunspot;


/// TODO Comment
Mesh::Mesh(const std::string &name, std::vector<Vertex> &v, std::vector<GLuint> &i, Material *material)
	: vertices{ v }
	, indices{ i }
	, transform{ math::Mat4::identity }
	, name_{ name }
	, material_{ material }
{
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

	glEnableVertexAttribArray(1); // Vertex Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2); // Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texCoords));

	glBindVertexArray(0); // Unbind vao

	std::cout << "Mesh: created " << name_ << std::endl; // TODO remove debug log
}


Mesh::~Mesh()
{
	if (material_ != nullptr) { delete material_; }
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);
	std::cout << "Mesh: destroyed " << name_ << std::endl; // TODO remove debug log
}


void Mesh::draw(const ShaderProgram *shader)
{
	// Bind transform matrix
	glUniformMatrix4fv(shader->getLocation("model"), 1, GL_FALSE, transform.matrix);

	if (material_ != nullptr) { material_->bind(shader); }

	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
