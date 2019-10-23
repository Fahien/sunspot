#pragma once

#include <unordered_map>

#include <gltfspot/Gltf.h>
#include <mathspot/Math.h>


#include "sunspot/graphics/Shader.h"
#include "sunspot/graphics/gl/Vao.h"
#include "sunspot/graphics/gl/Vbo.h"
#include "sunspot/graphics/Texture.h"

#include "view/GltfCamera.h"


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

	void draw( const shader::Program& shader, const gst::Shape& shape, const mst::Mat4& transform );

	void draw( const shader::Program& shader, const gst::Mesh::Primitive& primitive, const mst::Mat4& transform );

	void draw( const shader::Program& shader, const gst::Mesh& mesh, const mst::Mat4& transform );

	void draw( const shader::Program& shader, const gst::Node& node,
	           const mst::Mat4& transform = mst::Mat4::identity );

	void draw( const shader::Program& shader, const gst::Camera& camera,
	           const mst::Mat4& transform = mst::Mat4::identity );

	void draw( const shader::Program& shader, const gst::Light& light, const mst::Mat4& transform );

	Vbo& get_vbo( const gst::Gltf::Accessor& accessor, gst::Mesh::Primitive::Semantic semantic );
	Vbo& get_vbo( const gst::Box& box );

  private:
	Vao& get_vao( const gst::Mesh::Primitive& primitive );
	Vao& get_vao( const gst::Shape& shape );

	void bind( gst::Texture& texture );

	// Store current gltf
	gst::Gltf* gltf = nullptr;

	std::vector<Vao> shapes = {};

	std::unordered_map<gst::Camera*, std::unique_ptr<GltfCamera>> cameras = {};

	shader::Program shape_shader = { "shader/shape.vert", "shader/shape.frag" };

	// Cache textures
	std::unordered_map<gst::Texture*, Texture> textures;
	std::unordered_map<const gst::Mesh::Primitive*, Vao> vaos;
	std::unordered_map<const gst::Shape*, Vao> shape_vaos;

	/// Maps Gltf buffer view indices to VBOs
	std::unordered_map<size_t, Vbo> vbos;
	std::unordered_map<const gst::Shape*, Vbo> shape_vbos;


  public:
	bool render_bounds = false;
};

}  // namespace sunspot::graphics::gl
