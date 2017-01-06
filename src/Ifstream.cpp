#include "Ifstream.h"
#ifdef WIN32
#include <cstdlib>
#else
#include <libgen.h>
#endif


using namespace sunspot;


Ifstream::Ifstream(char *name)
	: std::ifstream{ name }
#ifdef WIN32
	, path_{}
#else
	, path_{ dirname(name) }
#endif
{
#ifdef WIN32
	char path[32];
	_splitpath_s(name, nullptr, 0, path, 32, nullptr, 0, nullptr, 0);
	path_ = path;
#endif
}


Ifstream::Ifstream(const std::string &name)
	: std::ifstream{ name }
#ifdef WIN32
	, path_{}
#else
	, path_{ dirname(const_cast<char *>(name.c_str())) }
#endif
{
#ifdef WIN32
	char path[32];
	_splitpath_s(name.c_str(), nullptr, 0, path, 32, nullptr, 0, nullptr, 0);
	path_ = path;
#endif
}
