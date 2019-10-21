#ifndef SUNSPOT_UTIL_H_
#define SUNSPOT_UTIL_H_

#include <vector>
#include <string>

namespace sunspot
{


inline std::vector<std::string> pack_args( const int argc, const char** argv )
{
	std::vector<std::string> args;

	for ( int i { 1 }; i < argc; ++i )
	{
		args.push_back( argv[i] );
	}

	return args;
}


template<typename T>
using Iterator = typename T::iterator;
template<typename T>
using ConstIterator = typename T::const_iterator;

template<typename Container, typename Type>
inline Iterator<Container> find( Container& container, const Type& val )
{
	return std::find( std::begin( container ), std::end( container ), val );
}

template<typename Container, typename Type>
inline ConstIterator<Container> find( const Container& container, const Type& val )
{
	return std::find( std::begin( container ), std::end( container ), val );
}


template<typename Container, typename Type>
inline bool contains( const Container& container, const Type& val )
{
	return std::find( std::begin( container ), std::end( container ), val ) != std::end( container );
}


class CliArgs
{
  public:
	CliArgs( const int argc, const char** argv );

  private:
	std::string find( const std::string& option ) const;

	std::string set_project_name();

	std::vector<std::string> args;

  public:
	struct {
		std::string name;
		std::string path;
		struct {
			std::string path;
		} config;
		struct {
			std::string path;
		} script;
	} project;

};

} // namespace sunspot

#endif // SUNSPOT_UTIL_H_
