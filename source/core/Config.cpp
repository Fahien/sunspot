#include "sunspot/core/Config.h"


namespace sunspot
{
Config::Config( dataspot::Dataspot& db ) : database{ db }, window{ /* .size = */ { queryWindowSize() } }
{
}


std::string Config::queryValue( const std::string& key ) const
{
	std::string query{ "SELECT value FROM main.config WHERE key = ?;" };

	auto& stmt = database.prepare( query );

	stmt.bind( key );
	stmt.step();
	auto value{ stmt.get_text( 0 ) };
	stmt.reset();

	return value;
}


mathspot::Size Config::queryWindowSize()
{
	return mathspot::Size{ /* .width  = */ std::stoi( queryValue( "window.width" ) ),
		                   /* .height = */ std::stoi( queryValue( "window.height" ) ) };
}


}  // namespace sunspot
