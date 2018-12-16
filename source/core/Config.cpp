#include "sunspot/core/Config.h"


namespace sunspot
{


Config::Config( dataspot::DataSpot& db )
	: database { db }
	, window{
		/* .size = */ { queryWindowSize() }
	}
{}



std::string Config::queryValue(const std::string& key) const
{

	std::string query { "SELECT value FROM main.config WHERE key = ?;" };

	auto& stmt = database.Prepare(query);

	stmt.Bind(key);
	stmt.Step();
	auto value { stmt.GetText(0) };
	stmt.Reset();

	return value;
}


mathspot::Size Config::queryWindowSize()
{
	return mathspot::Size{
		/* .width  = */ std::stoi( queryValue( "window.width" ) ),
		/* .height = */ std::stoi( queryValue( "window.height" ) )
	};
}


} // namespace sunspot
