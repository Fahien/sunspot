#include <string>

#include <gltfspot/Gltf.h>
#include <logspot/Logger.h>
#include <pyspot/Interpreter.h>

#include "view/GltfRenderer.h"

#include "sunspot/component/Camera.h"
#include "sunspot/core/Game.h"
#include "sunspot/system/graphic/Light.h"
#include "sunspot/system/graphic/Shader.h"
#include "view/GltfCamera.h"

#include "repository/ModelRepository.h"

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
	Size   window_size{ 1200, 720 };

	try
	{
		Interpreter interpreter{};
		Game        game;

		float aspect_ratio{ static_cast<float>( window_size.width ) / window_size.height };
		float fov{ radians( 45.0f ) };
		float near{ 0.125f };
		float far{ 256.0f };

		game.GetGraphics().SetViewport( graphic::System::Viewport{ { 0, 0 }, window_size } );

		Entity                       camera_entity{};
		component::PerspectiveCamera camera{ aspect_ratio, fov, far, near };
		camera.SetParent( &camera_entity );
		camera.Translate( Vec3{ 0.0f, 0.0f, -3.0f } );
		camera.SetAspectRatio( aspect_ratio );
		camera_entity.Add( camera );
		game.GetGraphics().SetCamera( camera_entity );

		// Load a Gltf model and upload the model into GPU buffers
		string          model_path{ argv[1] };
		ModelRepository model_repo{ "test" };
		int             model_id = 0;  // Just a custom id, not important for a test
		auto&           model    = model_repo.get_model( model_id, model_path );

		game.GetGraphics().AddModel( &model );

		Entity model_entity{};
		model_entity.Add( model );
		game.AddEntity( model_entity );

		/// Render to texture
		/// Compare it with a reference

		graphic::shader::Program base_program{ "shader/base.vert", "shader/base.frag" };
		game.GetGraphics().SetShaderProgram( &base_program );

		// DirectionalLight light{ Color{ 1.0f, 1.0f, 1.0f } };
		// light.SetDirection(0.0f, 0.0f, 4.0f);
		// float divFactor = 32.0f;
		// light.GetAmbient().r /= divFactor;
		// light.GetAmbient().g /= divFactor;
		// light.GetAmbient().b /= divFactor;
		// light.GetSpecular().r /= divFactor / 2;
		// light.GetSpecular().g /= divFactor / 2;
		// light.GetSpecular().b /= divFactor / 2;
		graphic::PointLight light{ Color{ 29.0f, 29.0f, 29.0f } };
		light.SetPosition( 1.0f, 1.0f, -2.0f );
		game.GetGraphics().SetLight( &light );

		// TODO render to texture
		// TODO Compare rendered texture to gold image

		game.Loop();
	} catch ( const graphic::Exception& e )
	{
		Logger::log.Error( "Exception: %s\n", e.what() );
		return EXIT_FAILURE;
	} catch ( const exception& e )
	{
		Logger::log.Error( "Exception: %s\n", e.what() );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
