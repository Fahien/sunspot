#include <iostream>
#include <string>

#include <Logger.h>

#include "Mesh.h"
#include "ShaderProgram.h"
#include "pyspot/component/Transform.h"

using namespace std;
using namespace sunspot;
using namespace pyspot::component;
namespace mst = mathspot;
namespace lst = logspot;


/// TODO Comment
Mesh::Mesh(const string&         name,
           vector<Vertex>&       v,
           vector<GLuint>&       i,
           shared_ptr<Material>& material)
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1); // Vertex Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2); // Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

	glBindVertexArray(0); // Unbind vao

	lst::Logger::log.info("Mesh: created %s\n", mName.c_str()); // TODO remove debug log
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mEbo);
	glDeleteBuffers(1, &mVbo);
	lst::Logger::log.info("Mesh: destroyed %s\n", mName.c_str()); // TODO remove debug log
}


void Mesh::ApplyTransform(Transform& pTransform)
{
	transform.ScaleX(pTransform.GetScale().GetX());
	transform.ScaleY(pTransform.GetScale().GetY());
	transform.ScaleZ(pTransform.GetScale().GetZ());
	transform.TranslateX(pTransform.GetPosition().GetX());
	transform.TranslateY(pTransform.GetPosition().GetY());
	transform.TranslateZ(pTransform.GetPosition().GetZ());
	transform.RotateX(pTransform.GetRotation().GetX());
	transform.RotateY(pTransform.GetRotation().GetY());
	transform.RotateZ(pTransform.GetRotation().GetZ());
}


void Mesh::Draw(const ShaderProgram& shader) const
{
	// Bind transform matrix
	glUniformMatrix4fv(shader.getLocation("model"), 1, GL_FALSE, transform.matrix);
	if (mMaterial)
	{
		mMaterial->bind(shader);
	}
	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
