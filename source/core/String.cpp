#include <stdexcept>
#include "core/String.h"


using namespace sunspot;

#ifdef ANDROID

JNIEnv* String::pEnv{ nullptr };


String::String(jstring jstr)
{
	if (!pEnv)
	{
		throw std::runtime_error("Please call String::Init(JNIEnv*) first");
	}
	auto cstr{ pEnv->GetStringUTFChars(jstr, 0) };
	assign(cstr);
	pEnv->ReleaseStringUTFChars(jstr, cstr);
}

#endif