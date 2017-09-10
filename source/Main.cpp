#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "Config.h"
#include "Logger.h"
#include "GlfwWindow.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Quad.h"
#include "Camera.h"
#include "Framebuffer.h"

#include "WavefrontObject.h"
#include "Mesh.h"
#include "Texture.h"

namespace sst = sunspot;
namespace mst = mathspot;

static int scale{ 2 };
static mst::Size windowSize{ 960, 540 };

static float fov{ 45.0f };
static float near{ 0.125f };
static float far{ 32.0f };

static const std::string tag{ "Main" };
static const std::string crateName{ "data/earth/earth.obj" };


void printLogo()
{
	sst::Logger::log.info("%s\n",
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
		for (int i{ 1 }; i < argc; ++i) {
			arguments.push_back(std::string{argv[i]});
		}

		// Window scale
		std::vector<std::string>::iterator it;
		if ((it = std::find(arguments.begin(), arguments.end(), "-scale")) != arguments.end()) {
			if (++it != arguments.end()) {
				scale = std::stoi(*it);
				sst::Logger::log.info("Scale [%d]\n", scale);
			}
			else { std::cerr << "Scale [" << scale << "] (Default)\n"; }
		}
		windowSize *= scale;

		
		bool decorated   { contains(arguments, "-decorated")    }; // Window decorations
		bool stereoscopic{ contains(arguments, "-stereoscopic") }; // Stereoscopic rendering

		sst::GlfwWindow window{ SST_TITLE, windowSize, decorated, stereoscopic };

		sst::Camera camera{ fov, static_cast<float>(windowSize.width) / windowSize.height, near, far };
		camera.setPosition(-2.0f, 0.0f, -6.0f);
		window.setCamera(&camera);

		sst::ShaderProgram baseProgram{ "shader/base.vert", "shader/base.frag" };
		window.setBaseProgram(&baseProgram);

		sst::Light light{ 0.5f, 0.5f, 0.5f };
		light.setPosition(2.0f, 2.0f, -4.0f);
		window.setLight(&light);

		// Inject dependencies into window
		sst::Framebuffer framebuffer{ window.getFrameSize() / 2 };
		sst::ShaderProgram quadProgram { "shader/quad.vert", "shader/quad.frag" };
		sst::ShaderProgram depthProgram{ "shader/quad.vert", "shader/depth.frag" };
		sst::Quad quad{};
		if (stereoscopic) {
			window.setQuadProgram(&quadProgram);
			window.setDepthProgram(&depthProgram);
			window.setQuad(&quad);
			window.setFramebuffer(&framebuffer);
		}

		// Load Wavefront Object
		sst::Ifstream is{ crateName };
		if (!is.is_open()) {
			sst::Logger::log.error("Could not find %s\n", crateName.c_str());
			return EXIT_FAILURE;
		}
		sst::WavefrontObject obj{};
		is >> obj;
		window.addObj(&obj);

		window.loop(); // Game loop

		sst::Logger::log.info("%s version %d.%d successful\n", SST_TITLE, SST_VERSION_MAJOR, SST_VERSION_MINOR);
		return EXIT_SUCCESS;
	}
	catch (const sst::GraphicException &e)
	{
		sst::Logger::log.error("%s: %s\n", tag.c_str(), e.what());
		return EXIT_FAILURE;
	}
	catch (const std::runtime_error &e)
	{
		sst::Logger::log.error("%s: %s\n", tag.c_str(), e.what());
		return EXIT_FAILURE;
	}
}
