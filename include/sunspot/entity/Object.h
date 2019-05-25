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
	Object( int id );
	Object( const std::string& name );
	Object( int id, const std::string& name );
	virtual ~Object()
	{
	}

	int GetId() const
	{
		return id;
	}

	void SetId( int id )
	{
		id = id;
	}

	Entity* GetParent() const
	{
		return parent;
	}

	const std::string& GetName() const
	{
		return name;
	}

	void SetName( const std::string& n )
	{
		name = n;
	}

	void SetParent( Entity* p )
	{
		parent = p;
	}

  private:
	int         id;
	std::string name;

	Entity* parent{ nullptr };
};

}  // namespace sunspot


#endif  // SST_OBJECT_H
