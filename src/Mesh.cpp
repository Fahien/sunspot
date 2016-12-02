#include "Mesh.h"
#include "ShaderProgram.h"
#include <string>


namespace sunspot {

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
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);
}


void Mesh::draw(const ShaderProgram *shader)
{
	GLuint diffuseCount{ 1 };
	GLuint specularCount{ 1 };
	for (GLuint i{ 0 }; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
		TextureType type{ textures[i].getType() };
		std::string name{ getTextureTypeName(type) };
		std::string number;
		if (type == DIFFUSE) { number = diffuseCount++; } // Transfer GLuint to stream
		else if (type == SPECULAR) { number = specularCount++; } // Transfer GLuint to stream
		shader->getLocation(("material." + name + number).c_str());
		glBindTexture(GL_TEXTURE_2D, textures[i].getId());
	}
	glActiveTexture(GL_TEXTURE0); // Reset active texture

	// Draw mesh
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}

