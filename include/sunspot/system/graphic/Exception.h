#pragma once

#include <stdexcept>

namespace sunspot::graphic
{


class Exception : public std::runtime_error
{
  public:
	Exception( const std::string& message ) : std::runtime_error( message ) {}
};


} // namespace sunspot::graphic
