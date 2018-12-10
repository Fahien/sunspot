#include <iostream>
#include <string>

#include <logspot/Logger.h>

#include "sunspot/component/Transform.h"
#include "Mesh.h"

using namespace std;
using namespace sunspot;
using namespace sunspot::component;
namespace mst = mathspot;
namespace lst = logspot;


/// TODO Comment
Mesh::Mesh(const string&         name,
           vector<Vertex>&       v,
           vector<GLuint>&       i,
           shared_ptr<graphic::Material>& material)
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

	lst::Logger::log.Info("Mesh: created %s", mName.c_str()); // TODO remove debug log
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mEbo);
	glDeleteBuffers(1, &mVbo);
	lst::Logger::log.Info("Mesh: destroyed %s", mName.c_str()); // TODO remove debug log
}


void Mesh::ApplyTransform(Transform& pTransform)
{
	transform.ScaleX(pTransform.scale.x);
	transform.ScaleY(pTransform.scale.y);
	transform.ScaleZ(pTransform.scale.z);
	transform.TranslateX(pTransform.position.x);
	transform.TranslateY(pTransform.position.y);
	transform.TranslateZ(pTransform.position.z);
	transform.RotateX(pTransform.rotation.x);
	transform.RotateY(pTransform.rotation.y);
	transform.RotateZ(pTransform.rotation.z);
}


void Mesh::Draw(const graphic::shader::Program& shader) const
{
	// Bind transform matrix
	glUniformMatrix4fv(shader.GetLocation("model"), 1, GL_FALSE, transform.matrix);
	if (mMaterial)
	{
		mMaterial->bind(shader);
	}
	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
