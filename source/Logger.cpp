#include "Logger.h"
#include "SunSpotConfig.h"

#ifdef SST_LOGGING
	#ifdef ANDROID
		#include <android/log.h>
		#define LOG_TAG ("SunSpoT")
	#else // other systems
		#include <iostream>
	#endif // ANDROID
#endif

using namespace sunspot;


Logger Logger::log{};


void Logger::vinfo(const char*
	#ifdef SST_LOGGING
		fmt
	#endif
	, std::va_list
	#ifdef SST_LOGGING
		args
	#endif
) const
{
#ifdef SST_LOGGING
	#ifdef ANDROID
		__android_log_vprint(ANDROID_LOG_VERBOSE, LOG_TAG, fmt, args);
	#else // other systems
		vfprintf(stdout, fmt, args);
	#endif // ANDROID
#endif // SST_LOGGING
}


void Logger::info(const char*
	#ifdef SST_LOGGING
		fmt
	#endif
	, ...) const
{
#ifdef SST_LOGGING
	std::va_list args;
	va_start(args, fmt);
	vinfo(fmt, args);
	va_end(args);
#endif // SST_LOGGING
}


void Logger::verror(const char*
	#ifdef SST_LOGGING
		fmt
	#endif
	, std::va_list
	#ifdef SST_LOGGING
		args
	#endif
) const
{
#ifdef SST_LOGGING
	#ifdef ANDROID
		__android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, fmt, args);
	#else // other systems
		vfprintf(stderr, fmt, args);
	#endif // ANDROID
#endif // SST_LOGGING
}


void Logger::error(const char*
	#ifdef SST_LOGGING
		fmt
	#endif
	, ...) const
{
#ifdef SST_LOGGING
	std::va_list args;
	va_start(args, fmt);
	verror(fmt, args);
	va_end(args);
#endif // SST_LOGGING
}
