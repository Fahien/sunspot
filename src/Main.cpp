#include <iostream>

#include "Config.h"
#include "SdlWindow.h"
#include "GlfwWindow.h"
#include "Light.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Quad.h"
#include "Camera.h"
#include "Framebuffer.h"


static constexpr unsigned width{ 960 * 2 };
static constexpr unsigned height{ 540 * 2};

static const std::string tag{ "Main" };

int main(int argc, char **argv)
{
	Window *window{ nullptr };

	try {
		if (argc > 1) {
			std::string firstArg{ argv[1] };
			if (firstArg == "-SDL") { window = new SdlWindow{ width, height, SST_TITLE }; }
		}
		if (window == nullptr) { window = new GlfwWindow{ width, height, SST_TITLE }; }


		ShaderProgram baseProgram{ "shader/base.vert", "shader/base.frag" };
		ShaderProgram depthProgram{ "shader/base.vert", "shader/depth.frag" };
		Light light{ 1.0f, 1.0f, 1.0f };
		light.setPosition(2.0f, 1.0f, -1.0f);
		Model model{ "shader/crate.bmp", .25f };
		Model room{ "shader/wall.bmp", 2.0f };
		Quad quad{};
		ShaderProgram quadProgram{ "shader/quad.vert", "shader/quad.frag" };
		Camera camera{ 45.0f, static_cast<float>(width) / height, 0.125f, 8.0f };
		Framebuffer framebuffer{ width, height / 2 };

		window->setBaseProgram(&baseProgram);
		window->setDepthProgram(&depthProgram);
		window->setLight(&light);
		window->setModel(&model);
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