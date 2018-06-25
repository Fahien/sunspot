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
	void SetEnv(JNIEnv* env) { pEnv = env };
#endif
	String(jstring s);

  private:
#ifdef ANDROID
	static JNIEnv* pEnv{ nullptr };
#endif

};


}


#endif // SST_STRING_H_
