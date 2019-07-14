#pragma once

#include <nlohmann/json.hpp>

#include <mathspot/Math.h>

#include "util/Util.h"


namespace sunspot
{
class Config
{
  public:
	Config();

	Config( const CliArgs& cli_args );

	Config( const nlohmann::json& j );

  private:
	/// @return A value from the config table in the database
	/// @param[in] key The key of the entry to look for
	std::string query_value( const std::string& key ) const;

	/// @return The size of the window
	mathspot::Size query_window_size() const;

	/// @return The name of the project
	std::string query_project_name() const;

	const nlohmann::json& j;

  public:
	const struct Window
	{
		mathspot::Size size;
	} window;

	struct Project
	{
		std::string name;
	} project;
};


}  // namespace sunspot
