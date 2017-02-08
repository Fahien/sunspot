#include "Logger.h"
#include "Config.h"

#ifdef SST_LOGGING
#include <iostream>
#include <cstdarg>
#endif

using namespace sunspot;

void Logger::info(const char *
#ifdef SST_LOGGING
	fmt
#endif
	...) const
{
#ifdef SST_LOGGING
	std::va_list arg;
	va_start(arg, fmt);
	vfprintf(stdout, fmt, arg);
	va_end(arg);
#endif
}


void Logger::error(const char *
#ifdef SST_LOGGING
	fmt
#endif
	...) const
{
#ifdef SST_LOGGING
	std::va_list arg;
	va_start(arg, fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);
#endif
}
