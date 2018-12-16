#include "sunspot/util/Util.h"

#include <logspot/Logger.h>


namespace sunspot
{


CliArgs::CliArgs( const int argc, const char** argv )
: m_Args { pack_args( argc, argv ) }
, project { /* .name = */ find( "-project" ) }
{
	logspot::Logger::log.Info( "Project [%s]", project.name.c_str() );
}


std::string CliArgs::find( const std::string& option ) const
{
	auto it { sunspot::find( m_Args, option ) };

	if ( it != m_Args.end() )
	{
		if ( ++it != m_Args.end() )
		{
			return *it;
		}
	}

	return {};
}

} // namespace sunspot