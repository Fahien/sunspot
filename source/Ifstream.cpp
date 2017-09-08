#include "Ifstream.h"
#include <string>
#ifdef WIN32
	#include <cstdlib>
	#include <iostream>
#else // other systems
	#include <libgen.h>
#endif


using namespace sunspot;


Ifstream::Ifstream(char* name)
#ifdef ANDROID
:	mFile{ AssetManager::assets.Open(name) }
,	mStream{ mFile.GetContent() }
#else
:	std::ifstream{ name }
#endif
#ifdef WIN32
,	mPath{}
#else // other systems
,	mPath{ dirname(name) }
#endif
{
#ifdef WIN32
	char path[32];
	_splitpath_s(name, nullptr, 0, path, 32, nullptr, 0, nullptr, 0);
	mPath = path;
#endif
}


Ifstream::Ifstream(const std::string& name)
#ifdef ANDROID
:	mFile{ AssetManager::assets.Open(name) }
,	mStream{ mFile.GetContent() }
#else
:	std::ifstream{ name }
#endif
#ifdef WIN32
,	mPath{}
#else // other systems
,	mPath{ dirname(const_cast<char*>(name.c_str())) }
#endif
{
#ifdef WIN32
	char path[32];
	_splitpath_s(name.c_str(), nullptr, 0, path, 32, nullptr, 0, nullptr, 0);
	mPath = path;
#endif
}


bool Ifstream::IsOpen() const
{
#ifdef ANDROID
	return true;
#else
	return is_open();
#endif
}


bool Ifstream::IsEof() const
{
#ifdef ANDROID
	return mStream.eof();
#else
	return eof();
#endif
}


std::string Ifstream::GetLine()
{
#ifdef ANDROID
	const size_t size{ 128 };
	char buffer[size];
	mStream.getline(buffer, size);
	std::string line{ buffer };
#else // other systems
	std::string line;
	std::getline(*this, line);
#endif
	return line;
}
