#include <fstream>
#include <cstdlib>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#include <logspot/Logger.h>
#include <dataspot/DataSpot.h>
#include <dataspot/Exception.h>

#include "sunspot/util/Util.h"
#include "sunspot/core/Config.h"
#include "SunSpotConfig.h"

#include "sunspot/core/Game.h"

#include "sunspot/system/graphic/Light.h"
#include "sunspot/system/graphic/Shader.h"
#include "sunspot/system/graphic/Framebuffer.h"
#include "Quad.h"

#include "WavefrontObject.h"
#include "Mesh.h"
#include "sunspot/system/graphic/Texture.h"
#include "sunspot/entity/Entity.h"
#include "repository/ModelRepository.h"
#include "repository/EntityRepository.h"

#include "sunspot/component/Camera.h"
#include "sunspot/core/GlfwWindow.h"
#include "sunspot/core/Gui.h"
#include "view/GltfCamera.h"

using namespace std;
using namespace pyspot;
using namespace mathspot;
using namespace logspot;

static int scale{ 1 };
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
	Logger::log.Info("%s\n",
" ________  ___  ___  ________   ________  ________  ________  ___________ \n\
|\\   ____\\|\\  \\|\\  \\|\\   ___  \\|\\   ____\\|\\   __  \\|\\   __  \\|\\____   ___\\ \n\
\\ \\  \\___|\\ \\  \\ \\  \\ \\  \\  \\  \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\|___ \\  \\__| \n\
 \\ \\_____  \\ \\  \\ \\  \\ \\  \\  \\  \\ \\_____  \\ \\   ____\\ \\  \\ \\  \\   \\ \\  \\  \n\
  \\|____|\\  \\ \\  \\ \\  \\ \\  \\  \\  \\|____|\\  \\ \\  \\___|\\ \\  \\ \\  \\   \\ \\  \\ \n\
    ____\\_\\  \\ \\_______\\ \\__\\  \\__\\____\\_\\  \\ \\__\\    \\ \\_______\\   \\ \\  \\\n\
   |\\_________\\|_______|\\|__| \\|__|\\_________\\|__|     \\|_______|    \\ \\__\\\n\
    \\|_________|                   \\|_________|                       \\|__| ");
}


int main( const int argc, const char **argv)
{
	using namespace sunspot;

	printLogo();

	try {
		// Get command line arguments
		auto arguments = CliArgs( argc, argv );

		// Load database
		dataspot::DataSpot database { arguments.project.db.path };

		// Get config values
		Config config { database };

		// Script has to be initialized before loading the entities
		Script::Initialize( arguments.project.script.path );

		Game game;

		auto& graphic = game.GetGraphics();
		graphic.SetViewport( graphic::System::Viewport{ { 0, 0 }, config.window.size } );

		float aspectRatio { static_cast<float>( config.window.size.width ) / config.window.size.height };

		{
			GltfPerspectiveCamera camera{ aspectRatio, fov, kNear, kFar };
			camera.Translate(Vec3{ 0.0f, 0.0f, 32.0f });
			//camera.LookAt(0.0f, 0.0f, 0.0f);
			//game.GetGraphics().SetCamera( &camera );
		}

		graphic::shader::Program baseProgram { "shader/base.vert", "shader/base.frag" };
		game.GetGraphics().SetShaderProgram( &baseProgram );

		//DirectionalLight light{ Color{ 1.0f, 1.0f, 1.0f } };
		//light.SetDirection(0.0f, 0.0f, 4.0f);
		//float divFactor = 1.0f;
		//light.GetAmbient().r /= divFactor;
		//light.GetAmbient().g /= divFactor;
		//light.GetAmbient().b /= divFactor;
		//light.GetSpecular().r /= divFactor / 2;
		//light.GetSpecular().g /= divFactor / 2;
		//light.GetSpecular().b /= divFactor / 2;
		graphic::PointLight light { Color { 18.0f, 18.0f, 18.0f } };
		light.SetPosition(0.0f, 0.0f, 8.0f);
		game.GetGraphics().SetLight( &light );

		ModelRepository modelRepository { arguments.project.path };
		EntityRepository entityRepository { database, modelRepository };

		// Read a set of objects from database
		constexpr int entitiesCount = 4;
		// For every object get the name
		for ( int i{ 0 }; i < entitiesCount; ++i )
		{
			Entity* entity { entityRepository.LoadEntity( i + 1 ) };

			if ( auto camera = entity->Get<component::Camera>() )
			{
				if ( auto pPerspectiveCam = dynamic_cast<component::PerspectiveCamera*>( &camera->get() ) )
				{
					pPerspectiveCam->SetAspectRatio( aspectRatio );
				}
				game.GetGraphics().SetCamera( *entity );
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
			// Main menu
			if (BeginMainMenuBar())
			{
				if (BeginMenu("File"))
				{
					if (MenuItem("Exit"))
					{
						glfwSetWindowShouldClose(game.GetWindow().GetHandle(), GLFW_TRUE);
					}
					EndMenu();
				}
				if (BeginMenu("Edit"))
				{
					EndMenu();
				}
			}
			EndMainMenuBar();
		};

		game.Loop(); // GameLoop.it


		Logger::log.Info( "%s version %d.%d successful", SST_TITLE, SST_VERSION_MAJOR, SST_VERSION_MINOR );
		return EXIT_SUCCESS;
	}
	catch (const graphic::Exception& e)
	{
		Logger::log.Error("%s: %s", tag.c_str(), e.what());
	}
	catch (const dst::Exception& e)
	{
		Logger::log.Error("%s: %s", tag.c_str(), e.ToString());
	}
	catch (const runtime_error& e)
	{
		Logger::log.Error("%s: %s", tag.c_str(), e.what());
	}
	return EXIT_FAILURE;
}
