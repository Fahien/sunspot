#include <jni.h>
#include <core/String.h>
#include <logspot/Logger.h>
#include <filespot/Asset.h>
#include <filespot/File.h>
#include <dataspot/DataSpot.h>

#include "core/Renderer.h"
#include "entity/Script.h"
#include "sunspot/input/Input.h"
#include "core/android/App.h"

using namespace sunspot;
using namespace android;
namespace lst = logspot;
namespace fst = filespot;


App::~App()
{
	lst::Logger::log.Info("App Destroyed");
}


static void printGlString(const char* name, GLenum s)
{
	const char* v{ (const char*)glGetString(s) };
	lst::Logger::log.Info("GL %s: %s", name, v);
}


App& App::Get(android_app* app)
{
	static App instance{ app };

	if (app && app != instance.mApp)
	{
		instance.mApp     = app;
		app->onAppCmd     = handleCommand;
		app->onInputEvent = handleInput;
	}

	return instance;
}

App::App(android_app* app)
: mApp{ app }
{
	app->onAppCmd     = handleCommand;
	app->onInputEvent = handleInput;

	fst::Asset::Init(app->activity->assetManager);

	// Get cache path
	auto env = app->activity->env;
	app->activity->vm->AttachCurrentThread(&env, nullptr);
	jclass activityClass = env->GetObjectClass(app->activity->clazz);
	jmethodID getCacheDir = env->GetMethodID(activityClass, "getCacheDir", "()Ljava/io/File;");
	jobject file = env->CallObjectMethod(app->activity->clazz, getCacheDir);
	jclass fileClass = env->FindClass("java/io/File");
	jmethodID getAbsolutePath = env->GetMethodID(fileClass, "getAbsolutePath", "()Ljava/lang/String;");
	jstring jpath = (jstring)env->CallObjectMethod(file, getAbsolutePath);
	mCachePath = env->GetStringUTFChars(jpath, nullptr);
	app->activity->vm->DetachCurrentThread();
	lst::Logger::log.Info("Cache %s", mCachePath.c_str());

	sqlite3_temp_directory = sqlite3_mprintf("%s", mCachePath.c_str());

	// Get internal path
	mInternalPath = app->activity->internalDataPath;

	// Get external path
	mExternalPath = app->activity->externalDataPath;

	// Copy python libs
	std::string libName{ "stdlib.zip" };
	std::string libPath{ mInternalPath + "/" + libName };
	if (!fst::File::Exists(libPath))
	{
		lst::Logger::log.Info("Copying lib %s", libPath.c_str());
		std::ofstream ofs{ libPath };
		if (ofs.is_open())
		{
			fst::Asset lib{ libName };
			ofs.write(lib.GetContent(), lib.GetLength());
			ofs.close();
		}
	}

	// Copy python scripts
	std::string scriptName{ "script.zip" };
	std::string scriptPath{ mInternalPath + "/" + scriptName };
	
	lst::Logger::log.Info("copying lib %s", scriptPath.c_str());
	std::ofstream ofs{ scriptPath };
	if (ofs.is_open())
	{
		fst::Asset script{ scriptName };
		ofs.write(script.GetContent(), script.GetLength());
		ofs.close();
	}

	// Copy database to internal folder
	std::string dbName{ "pong.data" };
	mDbPath = mInternalPath + "/" + dbName;
	//if (!fst::File::Exists(mDbPath))
	{
		lst::Logger::log.Info("copying DB %s", mDbPath.c_str());
		std::ofstream ofs{ mDbPath };
		if (ofs.is_open())
		{
			// Open asset database
			fst::Asset db{ dbName };
			ofs.write(db.GetContent(), db.GetLength());
			lst::Logger::log.Info("DB:\n %s", db.GetContent());
			ofs.close();
		}
	}

	// Initialize PySpot
	std::string path{ libPath + ":" + scriptPath };
	Script::Initialize(path);

}


void App::Loop()
{
	lst::Logger::log.Info("Asset test.txt %s", fst::Asset::Exists("test.txt") ? "exists" : "does not exist");
	fst::Asset asset{ "test.txt" };
	lst::Logger::log.Info("Content: %s", asset.GetContent());

	while (true)
	{
		// Read all pending events.
		int id;
		int events;
		android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((id = ALooper_pollAll(mPause ? -1 : 0, NULL, &events, (void**)&source)) >= 0)
		{
			// Process this event.
			if (source != NULL) source->process(mApp, source);

			// Check if we are exiting
			if (mApp->destroyRequested)
			{
				Graphics::Get().Destroy();
				return;
			}
		}

		if (!mPause && mRenderer)
		{
			mRenderer->render();
			Graphics::Get().Swap();
		}
	}
}


void App::handleCommand(struct android_app* app, int32_t cmd)
{
	App::Get(app).Handle(static_cast<const Command>(cmd));
}


void App::Handle(const Command command)
{
	switch (command)
	{
		case Command::InitWindow:
		{
			// Init Display
			Graphics::Get(mApp->window);
			// Graphics
			printGlString("Version", GL_VERSION);
			printGlString("Vendor", GL_VENDOR);
			printGlString("Renderer", GL_RENDERER);
			printGlString("Extensions", GL_EXTENSIONS);

			glEnable(GL_DEPTH_TEST);

			if (mRenderer)
			{
				delete mRenderer;
			}
			mRenderer = Renderer::New(mDbPath);

			// Draw frame
			mRenderer->render();
			Graphics::Get().Swap();
			lst::Logger::log.Info("InitWindow");
			break;
		}
		case Command::LostFocus:
		{
			mPause = true;
			break;
		}
		case Command::GainedFocus:
		{
			mPause = false;
			break;
		}
		case Command::TermWindow:
		{
			// Destroy surface
			Graphics::Get().DestroySurface();
			mPause = true;
			lst::Logger::log.Info("TermWindow");
			break;
		}
		case Command::LowMemory:
		{
			Graphics::Get().Destroy();
			delete mRenderer;
			mRenderer = nullptr;
			break;
		}
		default:
		{
			lst::Logger::log.Info("OtherCommand");
			break;
		}
	}
}


int32_t App::handleInput(android_app* app, AInputEvent* input)
{
	// Type
	auto type = static_cast<input::Type>(AInputEvent_getType(input));
	
	// Action
	input::Action action{ input::Action::PRESS };
	if (type == input::Type::MOTION)
	{
		auto action = AMotionEvent_getAction(input);
		unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
		switch (flags) {
			case AMOTION_EVENT_ACTION_DOWN:
			{
				action = input::Action::PRESS;
				break;
			}
			case AMOTION_EVENT_ACTION_UP:
			{
				action = input::Action::RELEASE;
				break;
			}
			default: break;
		}
	}

	// Position
	auto x = AMotionEvent_getX(input, 0);
	auto y = AMotionEvent_getY(input, 0);
	math::Vec2 position{ x, y };

	input::Key key = input::Key::A;
	input::Input i{ type, key, action, position };

	if (App::Get(app).Handle(i))
	{
		return 1; // input handled
	}

	return 0; // not handled
}


bool App::Handle(input::Input& input)
{
	if (input.GetType() == input::Type::MOTION)
	{
		mRenderer->Handle(input);
		return true;
	}

	return false;
}
