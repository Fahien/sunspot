#include "sunspot/entity/Object.h"

using namespace sunspot;


Object::Object() {}


Object::Object( int id )
:	m_Id { id }
{}


Object::Object( std::string& name )
:	m_Name { name }
{}


Object::Object( int id, std::string& name )
:	m_Id   { id   }
,	m_Name { name }
{}
