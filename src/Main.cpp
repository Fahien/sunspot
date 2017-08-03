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

using namespace sunspot;

static int scale{ 2 };
static math::Size windowSize{ 960, 540 };

static float fov{ 45.0f };
static float near{ 0.125f };
static float far{ 32.0f };

static const std::string tag{ "Main" };
static const std::string crateName{ "data/monument/monument.obj" };


void printLogo()
{
	Logger::log.info("%s\n",
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
				Logger::log.info("Scale [%d]\n", scale);
			}
			else { std::cerr << "Scale [" << scale << "] (Default)\n"; }
		}
		windowSize *= scale;

		
		bool decorated{ contains(arguments, "-decorated") }; // Window decorations
		bool stereoscopic{ contains(arguments, "-stereoscopic") }; // Stereoscopic rendering

		GlfwWindow window{ SST_TITLE, windowSize, decorated, stereoscopic };

		Camera camera{ fov, static_cast<float>(windowSize.width) / windowSize.height, near, far };
		camera.setPosition(-2.0f, 0.0f, -6.0f);
		window.setCamera(&camera);

		ShaderProgram baseProgram{ "shader/base.vert", "shader/base.frag" };
		window.setBaseProgram(&baseProgram);

		Light light{ 0.5f, 0.5f, 0.5f };
		light.setPosition(2.0f, 2.0f, -4.0f);
		window.setLight(&light);

		// Inject dependencies into window
		Framebuffer framebuffer{ window.getFrameSize() / 2 };
		ShaderProgram quadProgram{ "shader/quad.vert", "shader/quad.frag" };
		ShaderProgram depthProgram{ "shader/quad.vert", "shader/depth.frag" };
		Quad quad{};
		if (stereoscopic) {
			window.setQuadProgram(&quadProgram);
			window.setDepthProgram(&depthProgram);
			window.setQuad(&quad);
			window.setFramebuffer(&framebuffer);
		}

		// Load Wavefront Object
		Ifstream is{ crateName };
		if (!is.is_open()) {
			Logger::log.error("Could not find %s\n", crateName.c_str());
			return EXIT_FAILURE;
		}
		WavefrontObject obj{};
		is >> obj;
		window.addObj(&obj);

		window.loop(); // Game loop

		Logger::log.info("%s version %d.%d successful\n", SST_TITLE, SST_VERSION_MAJOR, SST_VERSION_MINOR);
		return EXIT_SUCCESS;
	}
	catch (const GraphicException &e)
	{
		Logger::log.error("%s: %s\n", tag.c_str(), e.what());
		return EXIT_FAILURE;
	}
	catch (const std::runtime_error &e)
	{
		Logger::log.error("%s: %s\n", tag.c_str(), e.what());
		return EXIT_FAILURE;
	}
}
