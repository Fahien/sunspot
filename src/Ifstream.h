#ifndef SST_IFSTREAM_H
#define SST_IFSTREAM_H

#include <fstream>
#include <sstream>
#include "android/AssetManager.h"

namespace sunspot
{

class Ifstream : public std::ifstream
{
public:
	Ifstream(char* name);
	Ifstream(const std::string& name);

	bool IsOpen() const;
	bool IsEof() const;

	inline const std::string& getPath() const { return mPath; }
	std::string GetLine();

private:
#ifdef ANDROID
	Asset mFile;
	std::stringstream mStream;
#endif
	std::string mPath;
};

}

#endif // SST_IFSTREAM_H
