/* Level.cpp
Michael Zahniser, 07/08/07

Function definitions for the Level class, which stores the map of a single level.
*/

#include "Level.h"

#include "LevelChanges.h"

#include <stdexcept>
#include <algorithm>
#include <cmath>

using std::runtime_error;
using std::find;
using std::ceil;
using std::sqrt;
using std::fabs;



Level::Level()
	: index(255), tileTypes(NULL)
{
}



Level::Level(DataFile &in, const vector< SharedPtr<TileType> > &theTileTypes)
	: tileTypes(&theTileTypes)
{
	Read(in);
}



void Level::Read(DataFile &in)
{
	index = in.ReadInt();
	
	string tag;
	in>>tag;
	
	while(tag != "end")
	{
		if(tag == "map")
			map.Read(in);
		else if(tag == "entities")
			entities.Read(in);
		else if(tag == "trigger")
			TriggerTag(in.ReadInt());
	
		in>>tag;
	}
}



ostream &operator<<(ostream &out, const Level &level)
{
	// If this is just a placeholder, don't output it.
	if(level.index == 255)
		return out;
	
	out<<"level "<<level.Index()<<"\n";
	
	out<<level.map;
	
	out<<level.entities;
	
	// List all the switches that have been triggered.
	for(int i = 0; i < level.triggered.size(); ++i)
		if(level.triggered[i])
			out<<"\ttrigger "<<i<<"\n";
	
	out<<"end\n\n";
	
	return out;
}



void Level::WriteSaveData(ostream &out) const
{
	// If this is just a placeholder, don't output it.
	if(index == 255)
		return;
	
	out<<"level "<<Index()<<"\n";
	
	out<<entities;
	
	// List all the switches that have been triggered.
	for(int i = 0; i < triggered.size(); ++i)
		if(triggered[i])
			out<<"\ttrigger "<<i<<"\n";
	
	out<<"end\n\n";
}



const TileType &Level::operator()(int x, int y) const
{
	return GetTileType(map(x, y));
}



// Apply all changes in the LevelChanges singleton.
void Level::ApplyChanges()
{
	LevelChanges &changes = LevelChanges::GetInstance();

	// Remove objects.
	for(vector<const Entity *>::const_iterator it = changes.remove.begin(); it != changes.remove.end(); ++it)
	{
		EntityList::iterator findIt = find(entities.begin(), entities.end(), *it);
		if(findIt != entities.end())
			entities.erase(findIt);
	}

	// Add objects.
	for(vector<Entity::Ptr>::const_iterator it = changes.add.begin(); it != changes.add.end(); ++it)
		entities.push_back(*it);

	// Trigger switches.
	for(vector<unsigned char>::const_iterator it = changes.switches.begin(); it != changes.switches.end(); ++it)
		TriggerTag(*it);
	
	changes.Clear();
}



void Level::Update(const Level &level)
{
	if(index != level.index)
		throw runtime_error("Attempted to perform a level data update using data for a different level.");
	
	// Replace the old entity list with the new one.
	if(level.entities.size())
		entities = level.entities;
		
	// Replace the old map with the new one.
	if(level.map)
		map = level.map;
	
	// Copy the tile type pointer if it exists.
	if(level.tileTypes)
		tileTypes = level.tileTypes;
	
	// The trigger flags from the new level should always be copied.
	// This allows the flags to be cleared when loading a different saved game
	// or reloading the default entities.
	triggered = level.triggered;
}



void Level::TriggerTag(int fieldTag)
{
	if(fieldTag >= triggered.size())
		triggered.resize(fieldTag + 1, false);
		
	triggered[fieldTag] = true;
}



const TileType &Level::GetTileType(int index) const
{
	if(index >= tileTypes->size())
		throw runtime_error("Undefined tile type.");
		
	const TileType &tileType = *(*tileTypes)[index];
	
	int fieldTag = tileType.FieldTag();
	if((!fieldTag | (fieldTag >= triggered.size())) || !triggered[fieldTag])
		return tileType;
		
	return GetTileType(tileType.Transition());
}



float Level::LineOfSight(float fromX, float fromY, float toX, float toY) const
{
	// Get the distance to the target.
	float dx = toX - fromX;
	float dy = toY - fromY;
	
	int gridX = static_cast<int>(fromX);
	int gridY = static_cast<int>(fromY);
	int endX = static_cast<int>(toX);
	int endY = static_cast<int>(toY);
	
	// Check that we're in bounds.
	if((gridX < 0) | (gridY < 0) | (gridX >= map.Width()) | (gridY >= map.Height()) | 
			(endX < 0) | (endY < 0) | (endX >= map.Width()) | (endY >= map.Height()))
		return 0.0f;
		
	if((*this)(gridX, gridY).IsOpaque() | (*this)(endX, endY).IsOpaque())
		return 0.0f;
	
	int stepX = 1;
	int stepY = 1;
	
	// Find out how far we have to go to reach the tile grid borders.
	float offX = static_cast<float>(gridX + 1) - fromX;
	float offY = static_cast<float>(gridY + 1) - fromY;
	
	if(dx < 0.0f)
	{
		stepX = -1;
		offX = 1.0f - offX;
		dx = -dx;
	}
	if(dy < 0.0f)
	{
		stepY = -1;
		offY = 1.0f - offY;
		dy = -dy;
	}
	
	// Special case for vertical or horizontal.
	if(gridX == endX)
	{
		for(; gridY != endY; gridY += stepY)
			if((*this)(gridX, gridY).IsOpaque())
				return 0.0f;
		return 1.0f;
	}
	if(gridY == endY)
	{
		for(; gridX != endX; gridX += stepX)
			if((*this)(gridX, gridY).IsOpaque())
				return 0.0f;
		return 1.0f;
	}
	
	float distance = sqrt(dx * dx + dy * dy);
	float visibility = 1.0;
	
	while((gridX * stepX < endX * stepX) | (gridY * stepY < endY * stepY))
	{
		float timeX = offX / dx;
		float timeY = offY / dy;
		
		bool isBlocked = (*this)(gridX, gridY).IsOpaque();
		
		if(timeX < timeY)
		{
			if(isBlocked && (visibility -= distance * timeX) <= 0.0f)
				return 0.0f;

			offX = 1.0f;
			offY -= timeX * dy;
			gridX += stepX;
		}
		else
		{
			if(isBlocked && (visibility -= distance * timeY) <= 0.0f)
				return 0.0f;

			offY = 1.0f;
			offX -= timeY * dx;
			gridY += stepY;	
		}
	}
	
	return visibility;
}



