#include "sunspot/core/Config.h"

#include <filespot/Ifstream.h>
#include <logspot/Log.h>

#include "sunspot/util/Config.h"

namespace sunspot
{
/// @return A default config json
nlohmann::json default_json()
{
	return nlohmann::json::parse( sunspot::util::config );
}


/// @return A config file, creating it if not found
nlohmann::json load_json( const std::string& path )
{
	// read a JSON file
	nlohmann::json j;

	filespot::Ifstream i{ path };
	if ( i.IsOpen() )
	{
		i >> j;
	}
	else
	{
		j = default_json();
	}

	return j;
}


Config::Config()
    : Config{ default_json() }
{
}


Config::Config( const CliArgs& args )
    : Config{ load_json( args.project.config.path ) }
{
}


Config::Config( const nlohmann::json& jj )
    : j{ jj }
    , window{ /* .size = */ { query_window_size() } }
    , project{ /* .name = */ { query_project_name() } }
{
}


mathspot::Size Config::query_window_size() const
{
	return mathspot::Size{ /* .width  = */ j["window"]["size"][0].get<int>(),
		                   /* .height = */ j["window"]["size"][1].get<int>() };
}


std::string Config::query_project_name() const
{
	if ( j.count( "project" ) && j["project"].count( "name" ) )
	{
		return j["project"]["name"].get<std::string>();
	}

	return {};
}

}  // namespace sunspot
