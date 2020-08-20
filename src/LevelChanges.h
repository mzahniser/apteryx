/* LevelChanges.h
Michael Zahniser, 09/29/07

Header file for the LevelChanges singleton class, which stores a list of entities to
be added or removed and switches to be triggered in the current level. The level
changes will be applied by the engine once all actions for the current time step
are resolved.
*/

#ifndef __MZ_LEVEL_CHANGES_H__
#define __MZ_LEVEL_CHANGES_H__


#include "Entity.h"
#include "TileType.h"

#include <vector>

using std::vector;



class LevelChanges {
public:
	// Get the one and only instance of LevelChanges.
	static LevelChanges &GetInstance();
	
	// Add an entity. The given entity should be the result of a call to new;
	// LevelChanges will wrap it in an Entity::Ptr.
	void Add(Entity *entity);
	// Add an entity that is already wrapped in a SharedPtr.
	void Add(Entity::Ptr entity);
	// Remove an entity. If an entity is found matching the given pointer, it
	// will be removed.
	void Remove(const Entity *entity);
	
	// Trigger whatever field the given tile is a switch for. If the given
	// tile type is not a switch, this does nothing. Returns true if a switch
	// was triggered.
	bool Trigger(const TileType &tileType);
	
	// Clear any queued changes.
	void Clear();
	
	
private:
	LevelChanges() {}
	
	// Disallow copying.
	LevelChanges(const LevelChanges &);
	LevelChanges &operator=(const LevelChanges &);
	
	
private:
	vector<Entity::Ptr> add;
	vector<const Entity *> remove;
	
	vector<unsigned char> switches;


private:
	static LevelChanges instance;
	
	friend class Level;
};

#endif