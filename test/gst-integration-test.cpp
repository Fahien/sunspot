#include <Gltf.h>
#include <string>

#include "view/GltfRenderer.h"
#include "GlfwWindow.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "Camera.h"

using namespace std;
using namespace sunspot;
using namespace mathspot;
using namespace gltfspot;


/// Test GltfSpoT module with SunSpoT
int main(int argc, char** argv)
{
	/// Check arguments
	if (argc < 2)
	{
		cerr << "Usage: " << argv[0] << " modelPath" << endl;
		return EXIT_FAILURE;
	}

	string title{ argv[0] };
	Size windowSize{ 1200, 720 };

	try
	{
		GlfwWindow window{ title.c_str(), windowSize, true, false };

		float fov{ 45.0f };
		float near{ 0.125f };
		float far{ 256.0f };
		Camera camera{ fov, static_cast<float>(windowSize.width) / windowSize.height, near, far };
		camera.setPosition(-2.0f, 0.0f, -6.0f);
		window.setCamera(&camera);

		/// Load a Gltf model
		string modelPath{ argv[1] };
		Gltf model{ Gltf::Load(modelPath) };

		/// Upload the model into GPU buffers
		GltfRenderer renderer{ model };
		window.AddGltf(&renderer);
		/// Render to texture
		/// Compare it with a reference

		ShaderProgram baseProgram{ "shader/base.vert", "shader/base.frag" };
		window.setBaseProgram(&baseProgram);

		//DirectionalLight light{ Color{ 1.0f, 1.0f, 1.0f } };
		//light.SetDirection(0.0f, 0.0f, 4.0f);
		//float divFactor = 32.0f;
		//light.GetAmbient().r /= divFactor;
		//light.GetAmbient().g /= divFactor;
		//light.GetAmbient().b /= divFactor;
		//light.GetSpecular().r /= divFactor / 2;
		//light.GetSpecular().g /= divFactor / 2;
		//light.GetSpecular().b /= divFactor / 2;
		PointLight light{ Color{ 1.0f, 1.0f, 1.0f } };
		light.SetPosition(1.0f, 1.0f, -1.0f);
		window.setLight(&light);

		// TODO render to texture
		// TODO Compare rendered texture to gold image

		window.loop();
	}
	catch (const GraphicException& e)
	{
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}
	catch (const exception& e)
	{
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}