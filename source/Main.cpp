#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <vector>

#include <dataspot/Dataspot.h>
#include <dataspot/Exception.h>
#include <logspot/Logger.h>

#include "SunSpotConfig.h"
#include "sunspot/core/Config.h"
#include "sunspot/util/Util.h"

#include "sunspot/core/Game.h"

#include "Quad.h"
#include "sunspot/system/graphic/Framebuffer.h"
#include "sunspot/system/graphic/Light.h"
#include "sunspot/system/graphic/Shader.h"

#include "Mesh.h"
#include "WavefrontObject.h"
#include "repository/EntityRepository.h"
#include "repository/ModelRepository.h"
#include "sunspot/entity/Entity.h"
#include "sunspot/system/graphic/Texture.h"

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

void printLogo()
{
	Logger::log.Info( "%s\n",
	                  " ________  ___  ___  ________   ________  ________  ________  ___________ \n\
|\\   ____\\|\\  \\|\\  \\|\\   ___  \\|\\   ____\\|\\   __  \\|\\   __  \\|\\____   ___\\ \n\
\\ \\  \\___|\\ \\  \\ \\  \\ \\  \\  \\  \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\|___ \\  \\__| \n\
 \\ \\_____  \\ \\  \\ \\  \\ \\  \\  \\  \\ \\_____  \\ \\   ____\\ \\  \\ \\  \\   \\ \\  \\  \n\
  \\|____|\\  \\ \\  \\ \\  \\ \\  \\  \\  \\|____|\\  \\ \\  \\___|\\ \\  \\ \\  \\   \\ \\  \\ \n\
    ____\\_\\  \\ \\_______\\ \\__\\  \\__\\____\\_\\  \\ \\__\\    \\ \\_______\\   \\ \\  \\\n\
   |\\_________\\|_______|\\|__| \\|__|\\_________\\|__|     \\|_______|    \\ \\__\\\n\
    \\|_________|                   \\|_________|                       \\|__| " );
}


int main( const int argc, const char** argv )
{
	using namespace sunspot;

	printLogo();

	try
	{
		// Get command line arguments
		auto arguments = CliArgs( argc, argv );

		// Load database
		dataspot::Dataspot database{ arguments.project.db.path };

		// Get config values
		Config config{ database };

		// Script has to be initialized before loading the entities
		Script::Initialize( arguments.project.script.path );

		Game game;

		auto& graphics = game.GetGraphics();
		graphics.SetViewport( graphic::System::Viewport{ { 0, 0 }, config.window.size } );

		float aspect_ratio{ static_cast<float>( config.window.size.width ) / config.window.size.height };

		graphic::shader::Program base_program{ "shader/base.vert", "shader/base.frag" };
		graphics.SetShaderProgram( &base_program );

		graphic::PointLight light{ Color{ 18.0f, 18.0f, 18.0f } };
		light.SetPosition( 0.0f, 0.0f, 8.0f );
		graphics.SetLight( &light );

		ModelRepository  model_repo{ arguments.project.path };
		EntityRepository entity_repo{ database, model_repo };

		// Read a set of objects from database
		constexpr int entities_count = 4;
		// For every object get the name
		for ( int i{ 0 }; i < entities_count; ++i )
		{
			Entity* entity{ entity_repo.LoadEntity( i + 1 ) };

			if ( auto camera = entity->Get<component::Camera>() )
			{
				if ( auto perspective_cam = dynamic_cast<component::PerspectiveCamera*>( &camera->get() ) )
				{
					perspective_cam->SetAspectRatio( aspect_ratio );
				}
				graphics.SetCamera( *entity );
			}

			if ( auto model = entity->Get<component::Model>() )
			{
				game.GetGraphics().AddModel( &model->get() );
			}

			game.AddEntity( *entity );
		}

		// Set up editor GUI
		auto& gui = game.GetGui();

		gui.fPreDraw = [&game]() {
			using namespace ImGui;
			auto& g      = GetIO();
			auto& window = game.GetWindow();

			// Main menu
			if ( BeginMainMenuBar() )
			{
				if ( IsMouseDragging() )
				{
					auto  drag = GetMouseDragDelta();
					auto& pos  = window.GetPosition();
					window.SetPosition( pos.x + static_cast<int>( drag.x ), pos.y + static_cast<int>( drag.y ) );
				}
				if ( BeginMenu( "File" ) )
				{
					EndMenu();
				}
				if ( BeginMenu( "Edit" ) )
				{
					if ( MenuItem( "Toggle" ) )
					{
						window.ToggleMaximization();
					}
					EndMenu();
				}
				SetCursorPosX( window.getFrameSize().width - 41.0f );
				if ( MenuItem( "T" ) )
				{
					window.ToggleMaximization();
				}
				if ( MenuItem( "X" ) )
				{
					window.SetClosing( true );
				}
			}
			EndMainMenuBar();

			// Frame
			PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
			ImGuiWindowFlags frameFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			                              ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
			                              ImGuiWindowFlags_NoInputs;
			SetNextWindowPos( { 0.0f, 0.0f } );
			SetNextWindowSize( g.DisplaySize );
			SetNextWindowBgAlpha( 0.0f );
			Begin( "Frame", nullptr, frameFlags );
			PopStyleVar( 1 );
			End();
		};

		game.Loop();  // GameLoop.it


		Logger::log.Info( "%s version %d.%d successful", SST_TITLE, SST_VERSION_MAJOR, SST_VERSION_MINOR );
		return EXIT_SUCCESS;
	}
	catch ( const graphic::Exception& e )
	{
		Logger::log.Error( "%s: %s", tag.c_str(), e.what() );
	}
	catch ( const dst::Exception& e )
	{
		Logger::log.Error( "%s: %s", tag.c_str(), e.to_string().c_str() );
	}
	catch ( const runtime_error& e )
	{
		Logger::log.Error( "%s: %s", tag.c_str(), e.what() );
	}
	return EXIT_FAILURE;
}
