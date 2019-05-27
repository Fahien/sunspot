#include "sunspot/entity/Object.h"

using namespace sunspot;


Object::Object( const int id )
    : id{ id }
{
}


Object::Object( const std::string& name )
    : name{ name }
{
}


Object::Object( const int id, const std::string& name )
    : id{ id }
    , name{ name }
{
}
