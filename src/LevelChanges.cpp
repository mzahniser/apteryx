/* LevelChanges.cpp
Michael Zahniser, 09/29/07

Function definitions for the LevelChanges class.
*/

#include "LevelChanges.h"



// Get the one and only instance of LevelChanges.
LevelChanges &LevelChanges::GetInstance()
{
	return instance;
}



// Add an entity. The given entity should be the result of a call to new;
// LevelChanges will wrap it in an Entity::Ptr.
void LevelChanges::Add(Entity *entity)
{
	Add(Entity::Ptr(entity));
}



// Add an entity that is already wrapped in a SharedPtr.
void LevelChanges::Add(Entity::Ptr entity)
{
	add.push_back(entity);
}



// Remove an entity. If an entity is found matching the given pointer, it
// will be removed.
void LevelChanges::Remove(const Entity *entity)
{
	remove.push_back(entity);
}



// Trigger whatever field the given tile is a switch for. If the given tile
// type is not a switch, this does nothing.
bool LevelChanges::Trigger(const TileType &tileType)
{
	if(!tileType.IsSwitch())
		return false;
	
	switches.push_back(tileType.FieldTag());
	
	return true;
}



// Clear any queued changes.
void LevelChanges::Clear()
{
	add.clear();
	remove.clear();
	switches.clear();
}



// The one and only instance of LevelChanges.
LevelChanges LevelChanges::instance;