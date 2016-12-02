#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "Config.h"
#include "SdlWindow.h"
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
static int width{ 960 };
static int height{ 540 };

static const std::string tag{ "Main" };
static const std::string objName{ "shader/cube.obj" };
static const std::string testTexture{ "shader/test" };

int main(int argc, char **argv)
{
	Window *window{ nullptr };

	try {
		// Get command line arguments
		std::vector<std::string> arguments{};
		for (int i{ 1 }; i < argc; ++i) {
			arguments.push_back(std::string{argv[i]});
		}

		// Test window
		if (std::find(arguments.begin(), arguments.end(), "-test") != arguments.end()) {
			zoom = 1;
		}
		width *= zoom;
		height *= zoom;

		bool stereo{ false };
		// Window decorations
		bool decorated{ false };
		if (std::find(arguments.begin(), arguments.end(), "-decorated") != arguments.end()) {
			decorated = true;
		}

		// Create window
		if (std::find(arguments.begin(), arguments.end(), "-SDL") != arguments.end()) {
			window = new SdlWindow{ SST_TITLE, width, height, decorated };
		}
		else {
			window = new GlfwWindow{ SST_TITLE, width, height, stereo, decorated };
		}

		ShaderProgram baseProgram{ "shader/base.vert", "shader/base.frag" };
		ShaderProgram depthProgram{ "shader/base.vert", "shader/depth.frag" };
		Light light{ 0.5f, 0.5f, 0.5f };
		light.setPosition(2.0f, 1.0f, -1.0f);
		Model model{ "shader/crate", .25f };
		Model room{ "shader/wall", 2.0f };
		Quad quad{};
		ShaderProgram quadProgram{ "shader/quad.vert", "shader/quad.frag" };
		Camera camera{ 45.0f, static_cast<float>(width) / height, 0.125f, 8.0f };
		math::Size frameSize {window->getFrameSize()};
		Framebuffer framebuffer{ frameSize.width, frameSize.height / 2 };

		std::ifstream is{ objName };
		if (!is.is_open()) {
			std::cerr << "Could not find " << objName << std::endl;
			return EXIT_FAILURE;
		}
		WavefrontObject obj{};
		is >> obj;
		std::vector<Texture> textures{};
		textures.push_back(Texture{ testTexture, TextureType::DIFFUSE });
		Mesh mesh{ obj.getVertices(), obj.getIndices(), textures };

		window->setBaseProgram(&baseProgram);
		window->setDepthProgram(&depthProgram);
		window->setLight(&light);
		window->setModel(&model);
		window->setMesh(&mesh);
		window->setRoom(&room);
		window->setQuadProgram(&quadProgram);
		window->setQuad(&quad);
		window->setCamera(&camera);
		window->setFramebuffer(&framebuffer);

		window->loop();
		std::cout << "Test version " << SST_VERSION_MAJOR << "." << SST_VERSION_MINOR << " successfull" << std::endl;

		delete window;
		return EXIT_SUCCESS;
	}
	catch (GraphicException &e)
	{
		std::cerr << tag << ": " << e.what() << std::endl; // TODO remove debug log
		return EXIT_FAILURE;
	}
}

