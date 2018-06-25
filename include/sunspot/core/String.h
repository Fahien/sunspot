#ifndef SST_STRING_H_
#define SST_STRING_H_

#include <string>

#ifdef ANDROID
# include <jni.h>
#endif


namespace sunspot
{


class String : public std::string
{
  public:
#ifdef ANDROID
	static void Init(JNIEnv* env) { pEnv = env; }
	String(jstring s);
#endif

  private:
#ifdef ANDROID
	static JNIEnv* pEnv;
#endif

};


}


#endif // SST_STRING_H_
