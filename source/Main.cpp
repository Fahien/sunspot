#include <fstream>
#include <cstdlib>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#include <Logger.h>
#include <DataSpot.h>

#include "SunSpotConfig.h"
#include "GlfwWindow.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Quad.h"
#include "Camera.h"
#include "Framebuffer.h"

#include "WavefrontObject.h"
#include "Mesh.h"
#include "Texture.h"
#include "Entity.h"
#include "ModelRepository.h"
#include "EntityRepository.h"


namespace sst = sunspot;
namespace mst = mathspot;
namespace lst = logspot;
namespace dst = dataspot;

static int scale{ 1 };
static mst::Size windowSize{ 960, 540 };

static float fov{ 45.0f };
static float near{ 0.125f };
static float far{ 32.0f };

static const std::string tag{ "Main" };
static const std::string dataDir{ "data/" };
static const std::string objExt{ ".obj" };

static const std::string projectDir{ "project" };

void printLogo()
{
	lst::Logger::log.info("%s\n",
R"( ________  ___  ___  ________   ________  ________  ________  _________   
|\   ____\|\  \|\  \|\   ___  \|\   ____\|\   __  \|\   __  \|\___   ___\ 
\ \  \___|\ \  \\\  \ \  \\ \  \ \  \___|\ \  \|\  \ \  \|\  \|___ \  \_| 
 \ \_____  \ \  \\\  \ \  \\ \  \ \_____  \ \   ____\ \  \\\  \   \ \  \  
  \|____|\  \ \  \\\  \ \  \\ \  \|____|\  \ \  \___|\ \  \\\  \   \ \  \ 
    ____\_\  \ \_______\ \__\\ \__\____\_\  \ \__\    \ \_______\   \ \  \
   |\_________\|_______|\|__| \|__|\_________\|__|     \|_______|    \ \__\
    \|_________|                   \|_________|                       \|__| )");
}


bool contains(const std::vector<std::string> &arguments, const char *s)
{
	return std::find(arguments.begin(), arguments.end(), s) != arguments.end();
}


int main(int argc, char **argv)
{
	printLogo();

	try {
		// Get command line arguments
		std::vector<std::string> arguments{};
		for (int i{ 1 }; i < argc; ++i)
		{
			arguments.push_back(std::string{argv[i]});
		}

		// Window scale
		std::vector<std::string>::iterator it;
		if ((it = std::find(arguments.begin(), arguments.end(), "-scale")) != arguments.end())
		{
			if (++it != arguments.end())
			{
				scale = std::stoi(*it);
				lst::Logger::log.info("Scale [%d]\n", scale);
			}
			else
			{
				std::cerr << "Scale [" << scale << "] (Default)\n";
			}
		}
		windowSize *= scale;

		bool decorated   { contains(arguments, "-decorated")    }; // Window decorations
		bool stereoscopic{ contains(arguments, "-stereoscopic") }; // Stereoscopic rendering

		// Project name
		std::string projectName{ "default" };
		if ((it = std::find(arguments.begin(), arguments.end(), "-project")) != arguments.end())
		{
			if (++it != arguments.end())
			{
				projectName = *it;
			}
			lst::Logger::log.info("Project [%s]\n", projectName.c_str());
		}

		// Load database
		dst::DataSpot dataspot;
		dataspot.Open(projectDir + "/" + projectName + "/" + projectName + ".data");
		windowSize.width  = std::stoi(dataspot.GetConfigValue("window.width"));
		windowSize.height = std::stoi(dataspot.GetConfigValue("window.height"));
		decorated = true;

		sst::GlfwWindow window{ SST_TITLE, windowSize, decorated, stereoscopic };

		sst::Camera camera{ fov, static_cast<float>(windowSize.width) / windowSize.height, near, far };
		camera.setPosition(-2.0f, 0.0f, -6.0f);
		window.setCamera(&camera);

		sst::ShaderProgram baseProgram{ "shader/base.vert", "shader/base.frag" };
		window.setBaseProgram(&baseProgram);

		//sst::DirectionalLight light{ sst::Color{ 1.0f, 1.0f, 1.0f } };
		//light.SetDirection(0.0f, 0.0f, 4.0f);
		//float divFactor = 1.0f;
		//light.GetAmbient().r /= divFactor;
		//light.GetAmbient().g /= divFactor;
		//light.GetAmbient().b /= divFactor;
		//light.GetSpecular().r /= divFactor / 2;
		//light.GetSpecular().g /= divFactor / 2;
		//light.GetSpecular().b /= divFactor / 2;
		sst::PointLight light{ sst::Color{ 1.0f, 1.0f, 1.0f } };
		light.SetPosition(0.0f, 4.0f, 0.0f);
		window.setLight(&light);

		// Inject dependencies into window
		sst::Framebuffer framebuffer{ window.getFrameSize() / 2 };
		sst::ShaderProgram quadProgram { "shader/quad.vert", "shader/quad.frag" };
		sst::ShaderProgram depthProgram{ "shader/quad.vert", "shader/depth.frag" };
		sst::Quad quad{};
		if (stereoscopic)
		{
			window.setQuadProgram(&quadProgram);
			window.setDepthProgram(&depthProgram);
			window.setQuad(&quad);
			window.setFramebuffer(&framebuffer);
		}

		// Initialize PySpot
		std::wstring wProjectDir{ projectDir.begin(), projectDir.end() };
		std::wstring wProjectName{ projectName.begin(), projectName.end() };
		sst::Script::Initialize(L"/" + wProjectDir + L"/" + wProjectName + L"/script");

		std::string projectPath{ projectDir + "/" + projectName + "/" };
		sst::ModelRepository modelRepository{ dataspot, projectPath };
		sst::EntityRepository entityRepository{ dataspot, modelRepository };

		// Read a set of objects from dataspot
		constexpr size_t objectsCount = 3;
		// Create an array of Objects
		std::array<sst::WavefrontObject, objectsCount> objects{};
		// For every object get the name
		for (size_t i{ 0 }; i < objectsCount; ++i)
		{
			sst::Entity* entity{ entityRepository.LoadEntity(i+1) };
			//std::shared_ptr<sst::Entity> pEntity{ entity };
			window.AddEntity(entity);
		}

		window.loop(); // GameLoop.it


		lst::Logger::log.info("%s version %d.%d successful\n", SST_TITLE, SST_VERSION_MAJOR, SST_VERSION_MINOR);
		return EXIT_SUCCESS;
	}
	catch (const sst::GraphicException &e)
	{
		lst::Logger::log.error("%s: %s\n", tag.c_str(), e.what());
		return EXIT_FAILURE;
	}
	catch (const std::runtime_error &e)
	{
		lst::Logger::log.error("%s: %s\n", tag.c_str(), e.what());
		return EXIT_FAILURE;
	}
}
