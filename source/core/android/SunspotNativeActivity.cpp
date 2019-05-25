#include <logspot/Log.h>

#include "core/android/App.h"

using namespace sunspot::android;
namespace lst = logspot;


/// This is the main entry point of a native application that is using
/// android_native_app_glue. It runs in its own thread, with its own
/// event loop for receiving input events and doing other things.
void android_main(android_app* pApp)
{
	auto& app = App::Get(pApp);
	app.Loop();
}
