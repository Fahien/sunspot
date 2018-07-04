#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>

#include <logspot/Logger.h>
#include <filespot/Ifstream.h>
#include <filespot/File.h>
#include <filespot/Asset.h>
#include <dataspot/DataSpot.h>

#include "repository/ModelRepository.h"
#include "repository/EntityRepository.h"
#include "entity/Script.h"
#include "core/Renderer.h"
#include "core/SunSpot.h"
#include "core/String.h"

namespace sst = sunspot;
namespace lst = logspot;
namespace fst = filespot;
namespace dst = dataspot;


static void printGlString(const char* name, GLenum s)
{
	const char* v{ (const char*)glGetString(s) };
	lst::Logger::log.Info("GL %s: %s", name, v);
}


static sst::Renderer* gRenderer = nullptr;

extern "C"
{
JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_init(JNIEnv *env, jobject obj, jstring jexternal, jstring jcache, jobject assets);
JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_resize(JNIEnv *env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_step(JNIEnv *env, jobject obj);
};


JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_init(JNIEnv *env, jobject obj, jstring jexternal, jstring jcache, jobject assets)
{
	if (gRenderer)
	{
		delete gRenderer;
		gRenderer = nullptr;
	}

	fst::AssetManager::assets.Init(env, assets);
	sst::String::Init(env);

	sst::String external{ jexternal };
	lst::Logger::log.Info("External %s", external.c_str());

	// Copy python libs
	std::string libName{ "stdlib.zip" };
	std::string libPath{ external + "/" + libName };
	if (!fst::File::Exists(libPath))
	{
		lst::Logger::log.Info("copying lib %s", libPath.c_str());
		std::ofstream ofs{ libPath };
		if (!ofs.is_open())
		{
			return;
		}
		fst::Ifstream lib{ libName };
		ofs.write(lib.GetAsset().GetContent(), lib.GetAsset().GetLength());
		ofs.close();
	}

	// Copy python scripts
	std::string scriptName{ "script.zip" };
	std::string scriptPath{ external + "/" + scriptName };
	//if (!fst::File::Exists(scriptPath))
	{
		lst::Logger::log.Info("copying lib %s", scriptPath.c_str());
		std::ofstream ofs{ scriptPath };
		if (!ofs.is_open())
		{
			return;
		}
		fst::Ifstream script{ scriptName };
		ofs.write(script.GetAsset().GetContent(), script.GetAsset().GetLength());
		ofs.close();
	}

	// Set SQLite3 temp directory
	sst::String cache{ jcache };
	lst::Logger::log.Info("Cache %s", cache.c_str());
	sqlite3_temp_directory = sqlite3_mprintf("%s", cache.c_str());

	// Copy database to internal folder
	std::string dbName{ "pong.data" };
	std::string dbPath{ external + "/pong.data"};
	if (!fst::File::Exists(dbPath))
	{
		lst::Logger::log.Info("copying DB %s", dbPath.c_str());
		std::ofstream ofs{ dbPath };
		if (!ofs.is_open())
		{
			return;
		}
		// Open asset database
		fst::Ifstream db{ "pong.data" };
		lst::Logger::log.Info("Database %s", db.IsOpen() ? "open" : "not open");
		ofs.write(db.GetAsset().GetContent(), db.GetAsset().GetLength());
		ofs.close();
	}

	// Initialize PySpot
	std::string path{ libPath + ":" + scriptPath };
	sst::Script::Initialize(path);

	// Renderer
	printGlString("Version", GL_VERSION);
	printGlString("Vendor", GL_VENDOR);
	printGlString("Renderer", GL_RENDERER);
	printGlString("Extensions", GL_EXTENSIONS);

	glEnable(GL_DEPTH_TEST);

	const char *versionStr{ reinterpret_cast<const char *>(glGetString(GL_VERSION)) };
	if (strstr(versionStr, "OpenGL ES 3."))
	{
		gRenderer = sst::Renderer::New(dbPath);
	}
	else
	{
		lst::Logger::log.Error("Unsupported OpenGL ES version");
	}
}

JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_resize(JNIEnv *env, jobject obj, jint width, jint height)
{
	if (gRenderer)
	{
		gRenderer->resize(width, height);
	}
}

JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_step(JNIEnv *env, jobject obj)
{
	if (gRenderer)
	{
		gRenderer->render();
	}
}
