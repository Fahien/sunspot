#ifndef SST_LOGGER_H
#define SST_LOGGER_H


namespace sunspot
{

class Logger {
  public:
	void info(const char *fmt...) const;

	void error(const char *fmt...) const;

};

}

#endif // SST_LOGGER_H
