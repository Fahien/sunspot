#ifndef SST_OBJECT_H_
#define SST_OBJECT_H_


#include <string>


namespace sunspot
{

class Object
{
public:
	Object();
	Object(int id);
	Object(std::string& name);
	Object(int id, std::string& name);

	inline int GetId() { return mId; }
	inline void SetId(int id) { mId = id; }

	inline std::string& GetName() { return mName; }
	inline void SetName(std::string& name) { mName = name; }

private:
	int mId;
	std::string mName;
};

}


#endif // SST_OBJECT_H
