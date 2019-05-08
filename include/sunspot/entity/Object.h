#ifndef SST_OBJECT_H_
#define SST_OBJECT_H_


#include <string>


namespace sunspot
{

class Entity;

class Object
{
  public:
	Object();
	Object(int id);
	Object(std::string& name);
	Object(int id, std::string& name);
	virtual ~Object() {}

	int GetId() { return m_Id; }
	void SetId(int id) { m_Id = id; }
	Entity* GetParent() const { return m_pParent; }

	std::string& GetName() { return m_Name; }
	void SetName(std::string& name) { m_Name = name; }
	void SetParent( Entity* pParent ) { m_pParent = pParent; }

  private:
	int m_Id;
	std::string m_Name;

	Entity* m_pParent { nullptr };
};

}


#endif // SST_OBJECT_H
