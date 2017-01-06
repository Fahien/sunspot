#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "Config.h"
#include "GlfwWindow.h"
#include "Light.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Quad.h"
#include "Camera.h"
#include "Framebuffer.h"

#include "WavefrontObject.h"
#include "Mesh.h"
#include "Texture.h"

using namespace sunspot;


static int zoom{ 2 };
static math::Size windowSize{ 960, 540 };

static const std::string tag{ "Main" };
static const std::string objName{ "data/cube/cube.obj" };
static const std::string testTexture{ "shader/test" };


void printLogo()
{
	std::cout << "┌─┐┬ ┬┌┐┌┌─┐┌─┐┌─┐┌┬┐\n"
	             "└─┐│ ││││└─┐├─┘│ │ │ \n"
	             "└─┘└─┘┘└┘└─┘┴  └─┘ ┴ \n";
}


bool contains(const std::vector<std::string> &arguments, const char *s)
{
	return std::find(arguments.begin(), arguments.end(), s) != arguments.end();
}


int main(int argc, char **argv)
{
	printLogo();
	GlfwWindow *window{ nullptr };

	try {
		// Get command line arguments
		std::vector<std::string> arguments{};
		for (int i{ 1 }; i < argc; ++i) {
			arguments.push_back(std::string{argv[i]});
		}

		// Test window
		std::vector<std::string>::iterator it;
		if ((it = std::find(arguments.begin(), arguments.end(), "-zoom")) != arguments.end()) {
			if (++it != arguments.end()) {
				std::cout << "Zoom [" << *it << "]\n";
				zoom = std::stoi(*it);
			}
			else { std::cerr << "Zoom [" << zoom << "] (Default)\n"; }
		}
		windowSize *= zoom;

		// Window decorations
		bool decorated{ contains(arguments, "-decorated") };
		// Stereoscopic rendering
		bool stereoscopic{ contains(arguments, "-stereoscopic") };
		// Create window
		window = new GlfwWindow{ SST_TITLE, windowSize, decorated, stereoscopic };

		ShaderProgram baseProgram{ "shader/base.vert", "shader/base.frag" };
		Light light{ 0.5f, 0.5f, 0.5f };
		light.setPosition(2.0f, 1.0f, -1.0f);
		Model model{ "shader/crate", .25f };
		Model room{ "shader/wall", 2.0f };
		Quad quad{};
		ShaderProgram quadProgram{ "shader/quad.vert", "shader/quad.frag" };
		ShaderProgram depthProgram{ "shader/quad.vert", "shader/depth.frag" };
		Camera camera{ 45.0f, static_cast<float>(windowSize.width) / windowSize.height, 0.125f, 64.0f };
		math::Size frameSize{ window->getFrameSize() };
		Framebuffer framebuffer{ frameSize.width / 2, frameSize.height / 2};

		Ifstream is{ objName };
		if (!is.is_open()) {
			std::cerr << "Could not find " << objName << std::endl;
			return EXIT_FAILURE;
		}
		WavefrontObject obj{};
		is >> obj;
		Mesh *mesh{ obj.getMesh() };

		window->setBaseProgram(&baseProgram);
		window->setDepthProgram(&depthProgram);
		window->setLight(&light);
		window->setModel(&model);
		window->setMesh(mesh);
		window->setRoom(&room);
		window->setQuadProgram(&quadProgram);
		window->setQuad(&quad);
		window->setCamera(&camera);
		window->setFramebuffer(&framebuffer);

		std::cout << "Start looping\n";
		window->loop();
		std::cout << "Test version " << SST_VERSION_MAJOR << "." << SST_VERSION_MINOR << " successfull" << std::endl;

		delete window;
		return EXIT_SUCCESS;
	}
	catch (const GraphicException &e)
	{
		std::cerr << tag << ": " << e.what() << std::endl; // TODO remove debug log
		return EXIT_FAILURE;
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << tag << ": " << e.what() << std::endl; // TODO remove debug log
		return EXIT_FAILURE;
	}
}
