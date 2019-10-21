#pragma once

#include <unordered_map>

#include <gltfspot/Gltf.h>
#include <mathspot/Math.h>


#include "sunspot/graphics/Shader.h"
#include "sunspot/graphics/gl/Vao.h"
#include "sunspot/graphics/gl/Vbo.h"

#include "view/GltfCamera.h"
#include "view/GltfMesh.h"


namespace mst = mathspot;
namespace gst = gltfspot;


namespace sunspot::graphics::gl
{
class Renderer
{
  public:
	Renderer( gst::Gltf* g = nullptr );

	void set_gltf( gst::Gltf& g );

	gst::Gltf* get_gltf() { return gltf; }

	void draw( const shader::Program& shader );

	void draw( const shader::Program& shader, const gst::Material& material );

	void draw( const shader::Program& shader, const gst::Mesh::Primitive& primitive, const mst::Mat4& transform );

	void draw( const shader::Program& shader, const gst::Mesh& mesh, const mst::Mat4& transform );

	void draw( const shader::Program& shader, const gst::Node& node,
	           const mst::Mat4& transform = mst::Mat4::identity );

	void draw( const shader::Program& shader, const gst::Camera& camera,
	           const mst::Mat4& transform = mst::Mat4::identity );

	void draw( const shader::Program& shader, const gst::Light& light, const mst::Mat4& transform );

	Vbo& get_vbo( const gst::Gltf::Accessor& accessor, gst::Mesh::Primitive::Semantic semantic );

  private:
	Vao& get_vao( const gst::Mesh::Primitive& primitive );

	void bind( gst::Texture& texture );

	// TODO either GLTF and GLTF Meshes should go somewhere else
	gst::Gltf*                                                    gltf    = {};
	std::unordered_map<gst::Mesh*, GltfMesh>                      meshes  = {};
	std::vector<GltfPrimitive>                                    shapes  = {};
	std::unordered_map<gst::Camera*, std::unique_ptr<GltfCamera>> cameras = {};

	shader::Program shape_shader = { "shader/shape.vert", "shader/shape.frag" };

	// Cache textures
	std::unordered_map<gst::Texture*, Texture> textures;
	std::unordered_map<const gst::Mesh::Primitive*, Vao> vaos;

	/// Maps Gltf buffer view indices to VBOs
	std::unordered_map<size_t, Vbo> vbos;


  public:
	bool render_bounds = false;
};

}  // namespace sunspot::graphics::gl
