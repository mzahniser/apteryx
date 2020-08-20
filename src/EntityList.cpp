/* EntityList.cpp
Michael Zahniser, 08/15/07

Function definitions for the EntityList class.
*/

#include "EntityList.h"

#include "Hovercraft_Entity.h"
#include "Drone_Entity.h"
#include "Dead_Entity.h"
#include "ControlledDrone_Entity.h"
#include "Upgrade_Entity.h"



EntityList::EntityList()
{
}



EntityList::EntityList(DataFile &in)
{
	Read(in);
}



void EntityList::Read(DataFile &in)
{
	clear();
	
	while(ReadNextEntity(in)) {}
}



ostream &operator<<(ostream &out, const EntityList &entityList)
{
	out<<"\tentities";
	
	out.precision(2);
	// Write out the name and (x, y) location of all Entities in the list.
	for(EntityList::const_iterator it = entityList.begin(); it != entityList.end(); ++it)
		out<<**it;
	
	return out<<"\n\tend\n";
}



bool EntityList::ReadNextEntity(DataFile &in)
{
	// Make sure the constructors have been registered.
	if(constructors.empty())
		RegisterConstructors();
	
	string name;
	in>>name;
	
	if(name == "end")
		return false;
	
	Entity::ConstructorMap::const_iterator it = constructors.find(name);
	if(it == constructors.end())
	{
		// Flush any numbers after the unrecognized entity.
		for(int i = 0; i < 5; ++i)
			in.ReadFloat();
			
		// Return true because we aren't at the end of the list yet.
		return true;
	}
	
	float x = in.ReadFloat();
	float y = in.ReadFloat();
	
	Entity::Ptr entity = it->second.first(it->second.second, x, y);
	
	float vx = in.ReadFloat();
	float vy = in.ReadFloat();
	entity->SetVelocity(vx, vy);
	
	if(in.HasNumber())
		entity->SetSpriteFrame(in.ReadInt());
	
	push_back(entity);
	
	return true;
}


	
void EntityList::RegisterConstructors()
{
	Hovercraft_Entity::Register(constructors);
	Drone_Entity::Register(constructors);
	ControlledDrone_Entity::Register(constructors);
	Dead_Entity::Register(constructors);
	Upgrade_Entity::Register(constructors);
}



Entity::ConstructorMap EntityList::constructors;
