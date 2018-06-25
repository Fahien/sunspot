#include "sunspot/android/String.h"


using namespace sunspot;


String::String(jstring jstr)
{
	auto cstr{ env->GetStringUTFChars(jstr, 0) };
	
	pEnv->ReleaseStringUTFChars
}