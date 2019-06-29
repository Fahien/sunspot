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
#include "sunspot/util/Cube.h"
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


/// @return A database, creating it if not found
std::variant<dataspot::Database, dataspot::Error> load_database( const std::string& path )
{
	auto open_result = dataspot::Database::open( path );
	if ( auto error = std::get_if<dataspot::Error>( &open_result ) )
	{
		// Create if cannot open
		auto create_result = dataspot::Database::create( path );
		if ( auto error = std::get_if<dataspot::Error>( &create_result ) )
		{
			Log::error( "Cannot create data %s: %s", path.c_str(), error->get_message().c_str() );
			std::exit( EXIT_FAILURE );
		}
		return create_result;
	}

	return open_result;
}


int main( const int argc, const char** argv )
{
	using namespace sunspot;

	print_logo();

	try
	{
		// Get command line arguments
		auto arguments = CliArgs( argc, argv );

		// Load database
		auto load_result = load_database( arguments.project.db.path );
		auto database = std::get_if<dataspot::Database>( &load_result );

		// Get config values
		Config config{ *database };

		// Script has to be initialized before loading the entities
		Script::initialize( arguments.project.script.path );

		Game game;

		auto& graphics = game.get_graphics();
		graphics.set_viewport( graphics::Viewport{ { 0, 0 }, config.window.size } );

		float aspect_ratio{ static_cast<float>( config.window.size.width ) / config.window.size.height };

		graphics::shader::Program base_program{ "shader/base.vert", "shader/base.frag" };
		graphics.set_shader_program( &base_program );

		graphics::PointLight light{ Color{ 1.0f, 1.0f, 1.0f } };
		light.SetPosition( 0.0f, 1.0f, 1.0f );
		graphics.set_light( &light );

		ModelRepository  model_repo{ arguments.project.path };
		EntityRepository entity_repo{ *database, model_repo };

		// Read a set of objects from database
		constexpr int entities_count = 4;
		// For every object get the name
		for ( int i = 0; i < entities_count; ++i )
		{
			if ( auto entity = entity_repo.load_entity( i + 1 ) )
			{
				if ( auto camera = entity->get<component::Camera>() )
				{
					if ( auto perspective_cam = dynamic_cast<component::PerspectiveCamera*>( &camera->get() ) )
					{
						perspective_cam->SetAspectRatio( aspect_ratio );
					}
					graphics.set_camera( *entity );
				}

				if ( auto model = entity->get<component::Model>() )
				{
					game.get_graphics().add_model( &model->get() );
				}

				game.add( *entity );
			}
		}

		// Create default camera
		Entity camera_entity{};
		float  fov{ radians( 45.0f ) };
		float  near{ 0.125f };
		float  far{ 256.0f };

		component::PerspectiveCamera camera{ aspect_ratio, fov, far, near };
		camera.set_parent( camera_entity );
		camera.Translate( Vec3{ 2.0f, 2.0f, -5.0f } );
		camera.GetTransform().rotation.y = radians( 18.0f );
		camera.GetTransform().rotation.x = radians( -18.0f );
		camera.SetAspectRatio( aspect_ratio );
		camera_entity.add( camera );

		// Create model placeholder
		auto entity   = Entity( 0, "Placeholder" );
		auto gltf     = gltfspot::Gltf( "", nlohmann::json::parse( cube ) );
		auto renderer = GltfRenderer( std::move( gltf ) );
		auto model    = component::Model( renderer.GetGltf().GetNodes().back(), renderer );
		entity.add<component::Model>( model );
		game.get_graphics().add_model( &model );
		if ( game.get_scene().get_entities().size() == 0 )
		{
			game.get_graphics().set_camera( camera_entity );
			game.add( entity );
			game.add( camera_entity );
		}

		// Set up editor GUI
		auto& gui = game.get_gui();

		gui.fPreDraw = [&game]() {
			using namespace ImGui;

			// Main menu
			if ( BeginMainMenuBar() )
			{
				if ( BeginMenu( "Edit" ) )
				{
					EndMenu();
				}
			}
			EndMainMenuBar();

			// Scene
			PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
			ImGuiWindowFlags scene_flags =
			    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
			Begin( "Scene", nullptr, scene_flags );
			PopStyleVar();
			for ( auto& entity : game.get_scene().get_entities() )
			{
				Text( "%s", entity->get_name().c_str() );
			}
			End();
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
