#include "Object.h"

namespace sst = sunspot;


sst::Object::Object() {}


sst::Object::Object(int id)
:	mId{ id }
{}


sst::Object::Object(std::string& name)
:	mName{ name }
{}


sst::Object::Object(int id, std::string& name)
:	mId  { id   }
,	mName{ name }
{}
