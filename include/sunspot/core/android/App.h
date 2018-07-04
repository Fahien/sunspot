#ifndef SST_APP_H_
#define SST_APP_H_

#include <android/native_app_glue/android_native_app_glue.h>
#include "sunspot/input/Input.h"


namespace sunspot
{

class Renderer;

namespace android
{

class App
{
  public:
	enum class Command : int32_t
	{
		/// AInputQueue has changed
		/// android_app->inputQueue will be updated
		InputChanged = APP_CMD_INPUT_CHANGED,

		/// New ANativeWindow ready for use
		/// android_app->window will contain the new window surface
		InitWindow = APP_CMD_INIT_WINDOW,

		/// Existing ANativeWindow needs to be terminated
		/// android_app->window still contains the existing window
		TermWindow = APP_CMD_TERM_WINDOW,

		/// Current ANativeWindow has been resized
		/// Please redraw with its new size
		WindowResized = APP_CMD_WINDOW_RESIZED,

		/// Current ANativeWindow needs to be redrawn
		WindowRedrawNeeded = APP_CMD_WINDOW_REDRAW_NEEDED,

		/// Content area of the window has changed
		ContentRectChanged = APP_CMD_CONTENT_RECT_CHANGED,

		/// Activity window has gained input focus
		GainedFocus = APP_CMD_GAINED_FOCUS,

		/// Activity window has lost input focus
		LostFocus = APP_CMD_LOST_FOCUS,

		/// Current device configuration has changed
		ConfigChanged = APP_CMD_CONFIG_CHANGED,

		/// The system is running low on memory
		LowMemory = APP_CMD_LOW_MEMORY,

		/// Activity has been started
		Start = APP_CMD_START,

		/// Activity has been resumed
		Resume = APP_CMD_RESUME,

		/// Should generate a new saved state
		SaveState = APP_CMD_SAVE_STATE,

		/// Activity has been paused
		Pause = APP_CMD_PAUSE,

		/// Activity has been stopped
		Stop = APP_CMD_STOP,

		/// Activity is being destroyed
		/// Clean up and exit before proceeding
		Destroy = APP_CMD_DESTROY
	};

	static App& Get(android_app* app);

	const std::string& GetCachePath()    const { return mCachePath;    }
	const std::string& GetInternalPath() const { return mInternalPath; }
	const std::string& GetExternalPath() const { return mExternalPath; }

	void Loop();

	void Handle(const Command command);
	bool Handle(input::Input& input);


  private:
	App(android_app* app);
	~App();

	static void handleCommand(android_app* app, int32_t cmd);
	static int32_t handleInput(android_app* app, AInputEvent* input);

	android_app* mApp{ nullptr };

	std::string mCachePath   { "" };
	std::string mInternalPath{ "" };
	std::string mExternalPath{ "" };

	std::string mDbPath{ "" };
	Renderer* mRenderer{ nullptr };

	bool mPause{ false };
};

}

}

#endif // SST_APP_H_