#include <iostream>

#include "Config.h"
#include "SdlWindow.h"
#include "GlfwWindow.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Quad.h"


static constexpr unsigned width{ 960 * 2 };
static constexpr unsigned height{ 540 * 2 };

static const std::string tag{ "Main" };

int main(int argc, char **argv)
{
	Window *window{ nullptr };
	
	try {
		if (argc > 1) {
			std::string firstArg{ argv[1] };
			if (firstArg == "-SDL") { window = new SdlWindow{ width / 2, height / 2, SST_TITLE }; }
		}
		if (window == nullptr) { window = new GlfwWindow{ width, height, SST_TITLE }; }

		ShaderProgram baseProgram{ "shader/base.vert", "shader/base.frag" };
		ShaderProgram depthProgram{ "shader/base.vert", "shader/depth.frag" };
		Model model{};
		Quad quad{};
		ShaderProgram quadProgram{ "shader/quad.vert", "shader/quad.frag" };
		//window->render(baseProgram, depthProgram, model);
		//window->render(quadProgram, quad);
		window->render(baseProgram, depthProgram, model, quadProgram, quad);
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