#include "sunspot/util/Util.h"

#include <logspot/Log.h>


namespace sunspot
{
CliArgs::CliArgs( const int argc, const char** argv ) : args{ pack_args( argc, argv ) }
{
	project.name        = set_project_name();
	project.path        = "project/" + project.name;
	project.config.path = project.path + "/config.json";
	project.script.path = project.path + "/script";

	logspot::Log::info( "Project [%s]", project.name.c_str() );
}


std::string CliArgs::find( const std::string& option ) const
{
	auto it{ sunspot::find( args, option ) };

	if ( it != args.end() )
	{
		if ( ++it != args.end() )
		{
			return *it;
		}
	}

	return {};
}


std::string CliArgs::set_project_name()
{
	auto name = find( "-project" );
	if ( name.empty() )
	{
		// Set temporary name
		name = "cube";
	}
	return name;
}


}  // namespace sunspot
