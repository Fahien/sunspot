#pragma once

#include <vector>

#include <gltfspot/Mesh.h>

#include "graphics/gl/Gl.h"
#include "graphics/gl/Ebo.h"
#include "graphics/gl/Vbo.h"


namespace sunspot::graphics::gl
{
class Renderer;


/// @brief Vertex Array Object
class Vao
{
  public:
	Vao( Renderer& r, const gltfspot::Mesh::Primitive& p );

	Vao( Vao&& v );

	~Vao();

	void draw();

	bool has_vertex_colors = false;

  private:
	Renderer* renderer = nullptr;

	GLuint handle = 0;

	Ebo ebo;

	GLenum mode = 0;
	
	GLsizei vertex_count = 0;

};


} // namespace sunspot::graphis:gl
