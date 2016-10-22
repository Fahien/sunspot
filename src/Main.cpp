#include <iostream>

#include "Config.h"
#include "Window.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Quad.h"


constexpr unsigned width {960 * 2};
constexpr unsigned height {540 * 2};

int main(void)
{
	Window window {width, height, SST_TITLE};
	ShaderProgram program {"shader/depth.frag"};
	Model model {};
	Quad quad {};
	ShaderProgram quadProgram {"shader/quad.vert", "shader/quad.frag"};
	//window.render(program, model);
	//window.render(quadProgram, quad);
	window.render(program, model, quadProgram, quad);
	std::cout << "Test version " << SST_VERSION_MAJOR << "." << SST_VERSION_MINOR << " successfull" << std::endl;
}
