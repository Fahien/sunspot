#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <vector>

#include <dataspot/Database.h>
#include <dataspot/Exception.h>
#include <logspot/Log.h>

#include "SunSpotConfig.h"
#include "sunspot/core/Config.h"
#include "sunspot/util/Config.h"
#include "sunspot/util/Util.h"

#include "sunspot/core/Game.h"

#include "Quad.h"
#include "sunspot/graphics/Framebuffer.h"
#include "sunspot/graphics/Light.h"
#include "sunspot/graphics/Shader.h"

#include "Mesh.h"
#include "WavefrontObject.h"
#include "repository/EntityRepository.h"
#include "repository/ModelRepository.h"
#include "sunspot/entity/Entity.h"
#include "sunspot/graphics/Texture.h"

#include "sunspot/component/Camera.h"
#include "sunspot/core/GlfwWindow.h"
#include "sunspot/core/Gui.h"
#include "view/GltfCamera.h"

using namespace std;
using namespace pyspot;
using namespace mathspot;
using namespace logspot;

static int  scale{ 1 };
static Size windowSize{ 960, 540 };

static float fov{ 45.0f };
static float kNear{ 0.125f };
static float kFar{ 32.0f };

static const string tag{ "Main" };
static const string dataDir{ "data/" };
static const string objExt{ ".obj" };

static const string projectDir{ "project" };

void print_logo()
{
	Log::info( "%s\n",
	           " ________  ___  ___  ________   ________  ________  ________  ___________ \n\
|\\   ____\\|\\  \\|\\  \\|\\   ___  \\|\\   ____\\|\\   __  \\|\\   __  \\|\\____   ___\\ \n\
\\ \\  \\___|\\ \\  \\ \\  \\ \\  \\  \\  \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\|___ \\  \\__| \n\
 \\ \\_____  \\ \\  \\ \\  \\ \\  \\  \\  \\ \\_____  \\ \\   ____\\ \\  \\ \\  \\   \\ \\  \\  \n\
  \\|____|\\  \\ \\  \\ \\  \\ \\  \\  \\  \\|____|\\  \\ \\  \\___|\\ \\  \\ \\  \\   \\ \\  \\ \n\
    ____\\_\\  \\ \\_______\\ \\__\\  \\__\\____\\_\\  \\ \\__\\    \\ \\_______\\   \\ \\  \\\n\
   |\\_________\\|_______|\\|__| \\|__|\\_________\\|__|     \\|_______|    \\ \\__\\\n\
    \\|_________|                   \\|_________|                       \\|__| " );
}


/// @return A config file, creating it if not found
std::variant<nlohmann::json, dataspot::Error> load_config( const std::string& path )
{
	// read a JSON file
	nlohmann::json j;

	fst::Ifstream i{ path };
	if ( i.IsOpen() )
	{
		i >> j;
	}
	else
	{
		j = j.parse( sunspot::util::config );
	}

	return j;
}

gst::Gltf::Camera create_default_camera( const sunspot::Config& config )
{
	auto camera = gst::Gltf::Camera();
	camera.name = "Default";
	camera.type = gst::Gltf::Camera::Type::Perspective;

	float aspect_ratio{ static_cast<float>( config.window.size.width ) / config.window.size.height };
	camera.perspective.aspectRatio = aspect_ratio;

	float fov{ radians( 45.0f ) };
	camera.perspective.yfov = fov;

	float near{ 0.125f };
	camera.perspective.znear = near;

	float far{ 256.0f };
	camera.perspective.zfar = far;

	return camera;
}

int main( const int argc, const char** argv )
{
	using namespace sunspot;

	print_logo();

	try
	{
		// Get command line args
		auto args = CliArgs( argc, argv );

		// Load config
		auto load_result = load_config( args.project.config.path );
		auto config_file = std::get_if<nlohmann::json>( &load_result );

		// Get config values
		Config config{ *config_file };

		// Script has to be initialized before loading the entities
		Script::initialize( args.project.script.path );

		Game game;


		if ( args.project.name != "cube" )
		{
			auto gltf_path = args.project.path + "/" + args.project.name + ".gltf";
			auto gltf      = gst::Gltf::Load( gltf_path );

			auto& cameras = gltf.GetCameras();
			if ( cameras.empty() )
			{
				// Create default camera
				auto camera = create_default_camera( config );
				cameras.push_back( camera );

				// Create node for camera
				auto node        = gst::Gltf::Node();
				node.pCamera     = &cameras[0];
				node.name        = "Camera";
				node.translation = Vec3{ 1.0f, 1.0f, 3.0f };

				auto& nodes = gltf.GetNodes();
				nodes.push_back( node );

				auto scene = gltf.GetScene();
				scene->nodes_indices.push_back( nodes.size() - 1 );

				// Reload scene nodes
				gltf.load_nodes();
			}

			game.set_gltf( std::move( gltf ) );
		}

		auto& graphics = game.get_graphics();
		graphics.set_viewport( graphics::Viewport{ { 0, 0 }, config.window.size } );


		graphics::shader::Program base_program{ "shader/base.vert", "shader/base.frag" };
		graphics.set_shader_program( &base_program );

		graphics::PointLight light{ Color{ 1.0f, 1.0f, 1.0f } };
		light.SetPosition( 0.0f, 1.0f, 1.0f );
		graphics.set_light( &light );

		// Create default camera
		auto camera_entity = Entity{ 0, "Camera" };

		// component::PerspectiveCamera camera{ aspect_ratio, fov, far, near };
		// camera.set_parent( camera_entity );
		// camera.Translate( Vec3{ 1.0f, 1.0f, 3.0f } );
		// camera.SetAspectRatio( aspect_ratio );
		// camera_entity.add( camera );
		// game.get_graphics().set_camera( camera_entity );

		// Set up editor GUI
		auto& gui = game.get_gui();

		gui.fPreDraw = [&game]() {
			using namespace ImGui;
			// Scene
			SetNextWindowPos( ImVec2( 0, 0 ) );
			PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
			ImGuiWindowFlags scene_flags =
			    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;
			Begin( "Scene", nullptr, scene_flags );
			PopStyleVar();

			PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2, 2 ) );
			for ( auto& node : game.get_scene().nodes )
			{
				PushID( node );
				if ( TreeNode( node->name.c_str() ) )
				{
					// Transform
					PushID( &node->translation );
					if ( TreeNode( "Transform" ) )
					{
						DragFloat3( "Position", &node->translation.x, 0.25f, -32.0f, 32.0f, "%.1f" );
						DragFloat4( "Rotation", &node->rotation.x, 0.125f, -1.0f, 1.0f, "%.2f" );
						DragFloat3( "Scale", &node->scale.x, 0.05f, 0.05f, 4.0f, "%.2f" );
						TreePop();
					}
					PopID();

					// Camera
					if ( node->pCamera )
					{
						PushID( node->pCamera );
						if ( TreeNode( "Camera" ) )
						{
							if ( node->pCamera->type == gltfspot::Gltf::Camera::Type::Perspective )
							{
								DragFloat( "Y Fov", &node->pCamera->perspective.yfov, 0.125f, 0.0f, 3.25f, "%.2f" );
								DragFloat( "Aspect Ratio", &node->pCamera->perspective.aspectRatio, 0.125f, 0.125f, 2.0f,
								           "%.3f" );
								DragFloat( "Z Near", &node->pCamera->perspective.znear, 0.125f, 0.125f, 1.0f, "%.3f" );
								DragFloat( "Z Far", &node->pCamera->perspective.zfar, 1.0f, 2.0f, 256.0f, "%.0f" );
							}
							else
							{
								DragFloat( "X Mag", &node->pCamera->orthographic.xmag, 0.125f, 0.0f, 2.0f, "%.3f" );
								DragFloat( "Y Mag", &node->pCamera->orthographic.ymag, 0.125f, 0.0f, 2.0f, "%.3f" );
								DragFloat( "Z Near", &node->pCamera->orthographic.znear, 0.125f, 0.125f, 1.0f, "%.3f" );
								DragFloat( "Z Far", &node->pCamera->orthographic.zfar, 1.0f, 2.0f, 256.0f, "%.0f" );
							}

							TreePop();
						}
						PopID();
					}
					TreePop();
				}
				PopID();
			}
			PopStyleVar();

			End();  // Scene

			ShowDemoWindow();
		};

		game.loop();  // GameLoop.it


		Log::info( "%s version %d.%d successful", SST_TITLE, SST_VERSION_MAJOR, SST_VERSION_MINOR );
		return EXIT_SUCCESS;
	}
	catch ( const graphics::Exception& e )
	{
		Log::error( "%s: %s", tag.c_str(), e.what() );
	}
	catch ( const dst::Exception& e )
	{
		Log::error( "%s: %s", tag.c_str(), e.to_string().c_str() );
	}
	catch ( const runtime_error& e )
	{
		Log::error( "%s: %s", tag.c_str(), e.what() );
	}
	return EXIT_FAILURE;
}
