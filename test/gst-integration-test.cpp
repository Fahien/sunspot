#include <string>

#include <gltfspot/Gltf.h>
#include <pyspot/Interpreter.h>
#include <logspot/Logger.h>

#include "view/GltfRenderer.h"

#include "sunspot/core/Game.h"
#include "sunspot/system/graphic/Shader.h"
#include "sunspot/system/graphic/Light.h"
#include "view/GltfCamera.h"

using namespace std;
using namespace pyspot;
using namespace mathspot;
using namespace gltfspot;
using namespace logspot;


/// Test GltfSpoT module with SunSpoT
int main( int argc, char** argv )
{
	using namespace sunspot;

	/// Check arguments
	if ( argc < 2 )
	{
		Logger::log.Error( "Usage: %s <model/path>\n", argv[0] );
		return EXIT_FAILURE;
	}

	string title{ argv[0] };
	Size windowSize{ 1200, 720 };

	try
	{
		Interpreter interpreter {};
		Game game;
		GlfwWindow window { game, title.c_str(), windowSize, true, false };

		float aspectRatio{ static_cast<float>( windowSize.width ) / windowSize.height };
		float fov{ radians( 45.0f ) };
		float near{ 0.125f };
		float far{ 256.0f };

		GltfPerspectiveCamera camera{ aspectRatio, fov, near, far };
		camera.Translate( Vec3{ 0.0f, 0.0f, 6.0f } );
		window.SetCamera( &camera );

		// Load a Gltf model and upload the model into GPU buffers
		string modelPath{ argv[1] };
		Gltf gltf{ Gltf::Load( modelPath ) };
		GltfRenderer renderer{ move( gltf ) };
		window.AddGltf( &renderer );

		/// Render to texture
		/// Compare it with a reference

		graphic::shader::Program baseProgram { "shader/base.vert", "shader/base.frag" };
		window.setBaseProgram( &baseProgram );

		//DirectionalLight light{ Color{ 1.0f, 1.0f, 1.0f } };
		//light.SetDirection(0.0f, 0.0f, 4.0f);
		//float divFactor = 32.0f;
		//light.GetAmbient().r /= divFactor;
		//light.GetAmbient().g /= divFactor;
		//light.GetAmbient().b /= divFactor;
		//light.GetSpecular().r /= divFactor / 2;
		//light.GetSpecular().g /= divFactor / 2;
		//light.GetSpecular().b /= divFactor / 2;
		graphic::PointLight light{ Color{ 1.0f, 1.0f, 1.0f } };
		light.SetPosition( 4.0f, 1.0f, -2.0f );
		window.setLight( &light );

		// TODO render to texture
		// TODO Compare rendered texture to gold image

		game.Loop();
	}
	catch ( const graphic::Exception& e )
	{
		Logger::log.Error( "Exception: %s\n", e.what() );
		return EXIT_FAILURE;
	}
	catch ( const exception& e )
	{
		Logger::log.Error( "Exception: %s\n", e.what() );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}