#include "sunspot/core/Config.h"

#include <logspot/Log.h>

namespace sunspot
{
Config::Config( nlohmann::json& jj ) : j{ jj }, window{ /* .size = */ { query_window_size() } }
{
}


std::string Config::query_value( const std::string& key ) const
{
	//auto query = "SELECT value FROM main.config WHERE key = ?;";

	//auto prepare_result = database.prepare( query );
	//if ( auto error = std::get_if<dataspot::Error>( &prepare_result ) )
	//{
	//	logspot::Log::error( "Cannote query key %s: %s", key.c_str(), error->get_message().c_str() );
	//	return {};
	//}
	//auto statement = std::get_if<dataspot::Statement>( &prepare_result );

	//statement->bind( key );
	//statement->step();
	//auto value = statement->get_text();
	//statement->reset();

	//return value;
	return "";
}


mathspot::Size Config::query_window_size()
{
	return mathspot::Size{ /* .width  = */ j["window"]["size"][0].get<int>(),
		                   /* .height = */ j["window"]["size"][1].get<int>()};
}


}  // namespace sunspot
