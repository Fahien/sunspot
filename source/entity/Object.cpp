#include "sunspot/entity/Object.h"

using namespace sunspot;


Object::Object() {}


Object::Object(int id)
:	mId{ id }
{}


Object::Object(std::string& name)
:	mName{ name }
{}


Object::Object(int id, std::string& name)
:	mId  { id   }
,	mName{ name }
{}
