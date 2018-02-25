#include <iostream>
#include <string>

#include <Logger.h>

#include "Mesh.h"
#include "ShaderProgram.h"

namespace sst = sunspot;
namespace mst = mathspot;
namespace lst = logspot;


/// TODO Comment
sst::Mesh::Mesh(const std::string& name, std::vector<sst::Vertex>& v, std::vector<GLuint>& i, sst::Material* material)
:	vertices { v }
,	indices  { i }
,	transform{ mst::Mat4::identity }
,	mName    { name }
,	mMaterial{ material }
{
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);
	glGenBuffers(1, &mEbo);

	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(sst::Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sst::Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1); // Vertex Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sst::Vertex), (GLvoid*)offsetof(sst::Vertex, normal));

	glEnableVertexAttribArray(2); // Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sst::Vertex), (GLvoid*)offsetof(sst::Vertex, texCoords));

	glBindVertexArray(0); // Unbind vao

	lst::Logger::log.info("Mesh: created %s\n", mName.c_str()); // TODO remove debug log
}


sst::Mesh::~Mesh()
{
	if (mMaterial != nullptr)
		delete mMaterial;
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mEbo);
	glDeleteBuffers(1, &mVbo);
	lst::Logger::log.info("Mesh: destroyed %s\n", mName.c_str()); // TODO remove debug log
}


void sst::Mesh::Draw(const sst::ShaderProgram& shader) const
{
	// Bind transform matrix
	glUniformMatrix4fv(shader.getLocation("model"), 1, GL_FALSE, transform.matrix);
	if (mMaterial != nullptr) { mMaterial->bind(shader); }
	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
