#include <iostream>
#include <string>
#include "Mesh.h"
#include "ShaderProgram.h"


namespace sunspot {


/// TODO Comment
Mesh::Mesh(std::vector<Vertex> &v, std::vector<GLuint> &i, std::vector<Texture> &t)
	: vertices{ v }
	, indices{ i }
	, textures{ t }
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

	// TODO refactor that SHIT
	textures.push_back(Texture{"shader/test", TextureType::DIFFUSE});
	textures.push_back(Texture{"shader/test", TextureType::SPECULAR});
	material_.diffuseMap = textures[0].getId();
	material_.specularMap = textures[1].getId();
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);
}


void Mesh::draw(const ShaderProgram *shader)
{
	material_.bind(shader);

	// Draw mesh
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}

