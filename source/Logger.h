#ifndef SST_LOGGER_H
#define SST_LOGGER_H

#include <cstdarg>


namespace sunspot
{

class Logger
{
public:
	static Logger log;

	void info (const char *fmt, ...) const;
	void vinfo(const char* fmt, std::va_list args) const;

	void error (const char *fmt, ...) const;
	void verror(const char* fmt, std::va_list args) const;
};

}

#endif // SST_LOGGER_H
