#include <fstream>
#include <cstdlib>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#include <logspot/Logger.h>
#include <dataspot/DataSpot.h>
#include <dataspot/Exception.h>

#include "SunSpotConfig.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Quad.h"
#include "Camera.h"
#include "Framebuffer.h"

#include "WavefrontObject.h"
#include "Mesh.h"
#include "Texture.h"
#include "sunspot/entity/Entity.h"
#include "repository/ModelRepository.h"
#include "repository/EntityRepository.h"
#include "GlfwWindow.h"

using namespace std;
using namespace pyspot;

namespace sst = sunspot;
namespace mst = mathspot;
namespace lst = logspot;
namespace dst = dataspot;

static int scale{ 1 };
static mst::Size windowSize{ 960, 540 };

static float fov{ 45.0f };
static float near{ 0.125f };
static float far{ 32.0f };

static const string tag{ "Main" };
static const string dataDir{ "data/" };
static const string objExt{ ".obj" };

static const string projectDir{ "project" };

void printLogo()
{
	lst::Logger::log.Info("%s\n",
" ________  ___  ___  ________   ________  ________  ________  ___________ \n\
|\\   ____\\|\\  \\|\\  \\|\\   ___  \\|\\   ____\\|\\   __  \\|\\   __  \\|\\____   ___\\ \n\
\\ \\  \\___|\\ \\  \\ \\  \\ \\  \\  \\  \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\|___ \\  \\__| \n\
 \\ \\_____  \\ \\  \\ \\  \\ \\  \\  \\  \\ \\_____  \\ \\   ____\\ \\  \\ \\  \\   \\ \\  \\  \n\
  \\|____|\\  \\ \\  \\ \\  \\ \\  \\  \\  \\|____|\\  \\ \\  \\___|\\ \\  \\ \\  \\   \\ \\  \\ \n\
    ____\\_\\  \\ \\_______\\ \\__\\  \\__\\____\\_\\  \\ \\__\\    \\ \\_______\\   \\ \\  \\\n\
   |\\_________\\|_______|\\|__| \\|__|\\_________\\|__|     \\|_______|    \\ \\__\\\n\
    \\|_________|                   \\|_________|                       \\|__| ");
}


bool contains(const vector<string> &arguments, const char *s)
{
	return find(arguments.begin(), arguments.end(), s) != arguments.end();
}


int main(int argc, char **argv)
{
	printLogo();

	try {
		// Get command line arguments
		vector<string> arguments{};
		for (int i{ 1 }; i < argc; ++i)
		{
			arguments.push_back(string{argv[i]});
		}

		// Window scale
		vector<string>::iterator it;
		if ((it = find(arguments.begin(), arguments.end(), "-scale")) != arguments.end())
		{
			if (++it != arguments.end())
			{
				scale = stoi(*it);
				lst::Logger::log.Info("Scale [%d]", scale);
			}
			else
			{
				cerr << "Scale [" << scale << "] (Default)\n";
			}
		}
		windowSize *= scale;

		bool decorated   { contains(arguments, "-decorated")    }; // Window decorations
		bool stereoscopic{ contains(arguments, "-stereoscopic") }; // Stereoscopic rendering

		// Project name
		string projectName{ "default" };
		if ((it = find(arguments.begin(), arguments.end(), "-project")) != arguments.end())
		{
			if (++it != arguments.end())
			{
				projectName = *it;
			}
			lst::Logger::log.Info("Project [%s]", projectName.c_str());
		}

		// Load database
		dst::DataSpot dataspot{ projectDir + "/" + projectName + "/" + projectName + ".data" };
		windowSize.width  = stoi(dataspot.GetConfigValue("window.width"));
		windowSize.height = stoi(dataspot.GetConfigValue("window.height"));
		decorated = true;

		// Initialize PySpot
		wstring wProjectDir;
		wProjectDir.assign(projectDir.begin(), projectDir.end());
		
		wstring wProjectName;
		wProjectName.assign(projectName.begin(), projectName.end());

		sst::Script::Initialize(wProjectDir + _T("/") + wProjectName + _T("/script"));

		sst::GlfwWindow window{ SST_TITLE, windowSize, decorated, stereoscopic };

		sst::Camera camera{ fov, static_cast<float>(windowSize.width) / windowSize.height, near, far };
		camera.setPosition(0.0f, -32.0f, 0.0f);
		camera.LookAt(0.0f, 0.0f, 0.0f);
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
		sst::PointLight light{ sst::Color{ 18.0f, 18.0f, 18.0f } };
		light.SetPosition(0.0f, 16.0f, 0.0f);
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

		string projectPath{ projectDir + "/" + projectName + "/" };
		sst::ModelRepository modelRepository{ projectPath };
		sst::EntityRepository entityRepository{ dataspot, modelRepository };

		// Read a set of objects from dataspot
		constexpr int entitiesCount = 3;
		// For every object get the name
		for (int i{ 0 }; i < entitiesCount; ++i)
		{
			sst::Entity* entity{ entityRepository.LoadEntity(i+1) };
			//shared_ptr<sst::Entity> pEntity{ entity };
			window.AddEntity(entity);
		}

		window.loop(); // GameLoop.it


		lst::Logger::log.Info("%s version %d.%d successful", SST_TITLE, SST_VERSION_MAJOR, SST_VERSION_MINOR);
		return EXIT_SUCCESS;
	}
	catch (const sst::GraphicException& e)
	{
		lst::Logger::log.Error("%s: %s", tag.c_str(), e.what());
	}
	catch (const dst::Exception& e)
	{
		lst::Logger::log.Error("%s: %s", tag.c_str(), e.ToString());
	}
	catch (const runtime_error& e)
	{
		lst::Logger::log.Error("%s: %s", tag.c_str(), e.what());
	}
	return EXIT_FAILURE;
}
