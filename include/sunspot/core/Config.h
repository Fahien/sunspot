#pragma once

#include <nlohmann/json.hpp>

#include <mathspot/Math.h>


namespace sunspot
{
class Config
{
  public:
	Config( nlohmann::json& j );

  private:
	/// @return A value from the config table in the database
	/// @param[in] key The key of the entry to look for
	std::string query_value( const std::string& key ) const;

	/// @return The size of the window
	mathspot::Size query_window_size();

	nlohmann::json& j;

  public:
	const struct
	{
		mathspot::Size size;
	} window;
};


}  // namespace sunspot
