#pragma once

#include <dataspot/Database.h>
#include <mathspot/Math.h>


namespace sunspot
{
class Config
{
  public:
	Config( dataspot::Database& db );

  private:
	/// @return A value from the config table in the database
	/// @param[in] key The key of the entry to look for
	std::string query_value( const std::string& key ) const;

	/// @return The size of the window
	mathspot::Size query_window_size();

	dataspot::Database& database;

  public:
	const struct
	{
		mathspot::Size size;
	} window;
};


}  // namespace sunspot
