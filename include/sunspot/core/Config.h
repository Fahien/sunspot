#pragma once

#include <dataspot/Dataspot.h>
#include <mathspot/Math.h>


namespace sunspot
{


class Config
{
  public:
	Config( dataspot::DataSpot& db );

private:
	/// @return A value from the config table in the database
	/// @param[in] key The key of the entry to look for
	std::string queryValue(const std::string& key) const;

	/// @return The size of the window
	mathspot::Size queryWindowSize();

  private:
	dataspot::DataSpot& database;

  public:
	const struct {
	  mathspot::Size size;
	} window;
};


} // namespace sunspot
