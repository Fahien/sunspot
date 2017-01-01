#include "Ifstream.h"
#include <libgen.h>


using namespace sunspot;


Ifstream::Ifstream(char *name)
	: std::ifstream{ name }
	, path_{ dirname(name) }
{}


Ifstream::Ifstream(const std::string &name)
	: std::ifstream{ name }
	, path_{ dirname(const_cast<char *>(name.c_str())) }
{}

