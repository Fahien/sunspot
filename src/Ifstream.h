#ifndef SST_IFSTREAM_H
#define SST_IFSTREAM_H

#include <fstream>

namespace sunspot
{

class Ifstream : public std::ifstream
{
public:
	Ifstream(char *name);
	Ifstream(const std::string &name);

	inline const std::string &getPath() const { return path_; }

private:
	const std::string path_;
};

}

#endif // SST_IFSTREAM_H

