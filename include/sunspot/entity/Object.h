#ifndef SST_OBJECT_H_
#define SST_OBJECT_H_


#include <string>


namespace sunspot
{
class Entity;

class Object
{
  public:
	Object( int id = 0 );
	Object( const std::string& name );
	Object( int id, const std::string& name );
	virtual ~Object() = default;

	int get_id() const { return id; }

	void set_id( int i ) { id = i; }

	Entity* get_parent() const { return parent; }

	const std::string& get_name() const { return name; }

	void set_name( const std::string& n ) { name = n; }

	void set_parent( Entity& p ) { parent = &p; }

  private:
	int         id;
	std::string name;

	Entity* parent{ nullptr };
};

}  // namespace sunspot


#endif  // SST_OBJECT_H
