#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>

#include <android/SunSpot.h>
#include <android/Renderer.h>
#include <logspot/Logger.h>
#include <filespot/Ifstream.h>
#include <filespot/AssetManager.h>
#include <DataSpot.h>

namespace sst = sunspot;
namespace lst = logspot;
namespace fst = filespot;
namespace dst = dataspot;


static void printGlString(const char* name, GLenum s)
{
	const char* v{ (const char*)glGetString(s) };
	lst::Logger::log.Info("GL %s: %s\n", name, v);
}


static sst::Renderer* gRenderer = nullptr;

extern "C"
{
JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_init(JNIEnv *env, jobject obj, jstring external, jstring cache, jobject assets);
JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_resize(JNIEnv *env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_step(JNIEnv *env, jobject obj);
};


JNIEXPORT void JNICALL Java_me_fahien_sunspot_SunSpotLib_init(JNIEnv *env, jobject obj, jstring external, jstring cache, jobject assets)
{
	if (gRenderer)
	{
		delete gRenderer;
		gRenderer = nullptr;
	}

	fst::AssetManager::assets.Init(env, assets);

	printGlString("Version", GL_VERSION);
	printGlString("Vendor", GL_VENDOR);
	printGlString("Renderer", GL_RENDERER);
	printGlString("Extensions", GL_EXTENSIONS);

	glEnable(GL_DEPTH_TEST);

	const char *versionStr{ reinterpret_cast<const char *>(glGetString(GL_VERSION)) };
	if (strstr(versionStr, "OpenGL ES 3."))
	{
		gRenderer = sst::Renderer::createRenderer();
	}
	else
	{
		lst::Logger::log.Error("Unsupported OpenGL ES version");
	}
	
	{
		fst::Ifstream db{ "pong.data", std::ios::binary };
		lst::Logger::log.Info("Database line: %s", db.GetAsset().GetContent());
	}

	// Open asset database
	fst::Ifstream db{ "pong.data" };
	lst::Logger::log.Info("Database %s", db.IsOpen() ? "open" : "not open");

	const char* nativeString = env->GetStringUTFChars(external, 0);
	std::string sexternal{ nativeString };
	env->ReleaseStringUTFChars(external, nativeString);
	lst::Logger::log.Info("External %s", sexternal.c_str());

	nativeString = env->GetStringUTFChars(cache, 0);
	std::string scache{ nativeString };
	env->ReleaseStringUTFChars(cache, nativeString);
	lst::Logger::log.Info("Cache %s", scache.c_str());
	sqlite3_temp_directory = sqlite3_mprintf("%s", scache.c_str());

	std::string dbPath{ sexternal + "/pong.data"};
	lst::Logger::log.Info("copying DB %s", dbPath.c_str());
	std::ofstream ofs{ dbPath };
	if (!ofs.is_open())
	{
		return;
	}
	ofs.write(db.GetAsset().GetContent(), db.GetAsset().GetLength());
	ofs.close();


	// Load database
	dst::DataSpot dataspot;
	dataspot.Open(dbPath);
	int width  = stoi(dataspot.GetConfigValue("window.width"));
	int height = stoi(dataspot.GetConfigValue("window.height"));
	lst::Logger::log.Info("DataSpot Size [%dx%d]", width, height);
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
