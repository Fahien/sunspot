#pragma once

#include <stdexcept>

namespace sunspot::graphics
{


class Exception : public std::runtime_error
{
  public:
	Exception( const std::string& message ) : std::runtime_error( message ) {}
};


} // namespace sunspot::graphics
