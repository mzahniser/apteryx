/* EntityList.cpp
Michael Zahniser, 08/15/07

Header file for the EntityList class, a vector of Entities that provides serialization
operators for writing map files or saved games.
*/

#ifndef __MZ_ENTITY_LIST_H__
#define __MZ_ENTITY_LIST_H__


#include "Entity.h"
#include "DataFile.h"

#include <vector>
#include <ostream>
#include <string>

using std::vector;
using std::ostream;



class EntityList : public vector<Entity::Ptr> {
public:
	EntityList();
	EntityList(DataFile &in);
	
	void Read(DataFile &in);
	friend ostream &operator<<(ostream &out, const EntityList &entityList);


private:
	bool ReadNextEntity(DataFile &in);

	static void RegisterConstructors();
	
	
private:
	static Entity::ConstructorMap constructors;
};

#endif