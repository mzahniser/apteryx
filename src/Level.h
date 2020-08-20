/* Level.h
Michael Zahniser, 07/08/07

Header file for the Level class, which stores the map of a single level
and all the entities in the level.
*/

#ifndef __MZ_LEVEL_H__
#define __MZ_LEVEL_H__


#include "Map.h"
#include "EntityList.h"
#include "TileType.h"
#include "SharedPtr.h"
#include "DataFile.h"

#include <ostream>
#include <vector>

using std::ostream;
using std::vector;



class Level {
public:
	Level();
	Level(DataFile &in, const vector< SharedPtr<TileType> > &theTileTypes);

	void Read(DataFile &in);
	friend ostream &operator<<(ostream &out, const Level &level);
	void WriteSaveData(ostream &out) const;
	
	const TileType &operator()(int x, int y) const;
	int Width() const { return map.Width(); }
	int Height() const { return map.Height(); }

	int Index() const { return static_cast<int>(index); }
	const EntityList &Entities() const { return entities; }
	EntityList &Entities() { return entities; }
	
	// Update the level data with the given new data (such as a saved game)
	void Update(const Level &level);
	
	// Apply all changes in the LevelChanges singleton.
	void ApplyChanges();
	
	// Check whether the line of sight between two points is clear.
	// The returned value is a fractional visibility between 0 and 1.
	float LineOfSight(float fromX, float fromY, float toX, float toY) const;
	
	
private:
	void TriggerTag(int fieldTag);
	const TileType &GetTileType(int index) const;
	
	
private:
	unsigned char index;
	Map map;
	EntityList entities;
	vector<bool> triggered;
	
	const vector< SharedPtr<TileType> > *tileTypes;
};

#endif