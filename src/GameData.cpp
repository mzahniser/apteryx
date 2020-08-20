/* GameData.cpp
Michael Zahniser, 09/20/07

Function definitions for the GameData class.
*/

#include "GameData.h"

#include "LevelChanges.h"
#include "Player.h"
#include "DataFile.h"
#include "SoundEngine.h"

#include <fstream>
#include <stdexcept>
#include <cmath>

using std::ofstream;
using std::runtime_error;
using std::fmod;



GameData &GameData::GetInstance()
{
	return instance;
}
	


void GameData::Initialize(const string &resourcePath)
{
	rootDirectory = resourcePath;
	
	levels.clear();
	tileTypes.clear();
	materials.clear();
	
	Read(rootDirectory + "data.txt");
	
	// Create the default material.
	materials.push_back(SharedPtr<Material>(new Material()));
	
	NewGame();
}



void GameData::NewGame()
{
	Read(rootDirectory + "entities.txt");
	
	FindPlayer();
}



void GameData::LoadGame(const string &path)
{
	Read(FullPath(path));
	
	FindPlayer();
}



void GameData::SaveGame(const string &path)
{
	ofstream out(FullPath(path).c_str());
			
	out<<Player::GetInstance();

	for(vector< SharedPtr<Level> >::const_iterator it = levels.begin(); 
			it != levels.end(); ++it)
		if(*it)
			(*it)->WriteSaveData(out);
}



void GameData::ExportMap(const string &path)
{	
	ofstream out(FullPath(path).c_str());
		
	for(vector< SharedPtr<Level> >::const_iterator it = levels.begin(); 
			it != levels.end(); ++it)
		if(*it)
			out<<**it;
	
	for(vector< SharedPtr<TileType> >::const_iterator it = tileTypes.begin(); 
			it != tileTypes.end(); ++it)
		if(*it)
			out<<**it;
	
	for(vector< SharedPtr<Material> >::const_iterator it = materials.begin(); 
			it != materials.end(); ++it)
		if(*it)
			out<<**it;
}



// Change the current level, and move the player to the same position, in the new level.
bool GameData::SetCurrentLevel(int index)
 {
	if((index < 0) | (index >= levels.size()))
		return false;
		
	LevelChanges &changes = LevelChanges::GetInstance();

	changes.Remove(&*player);
	CurrentLevel()->ApplyChanges();
	
	currentLevelIndex = index;
		
	changes.Add(player);
	CurrentLevel()->ApplyChanges();
	
	return true;
}



void GameData::SwitchControl()
{
	const Level &level = *CurrentLevel();
	
	// Find the currently controlled entity in the list.
	EntityList::const_iterator it = level.Entities().begin();
	for( ; it != level.Entities().end(); ++it)
		if(*it == controlledEntity)
			break;
	
	// If the currently controlled entity had been removed from the list,
	// switch back to player control.
	if(it == level.Entities().end())
	{
		controlledEntity = player;
		return;
	}
	
	// Find the next controllable entity in the level.
	++it;
	for( ; it != level.Entities().end(); ++it)
		if((*it)->IsControlled())
		{
			controlledEntity = *it;
			return;
		}
	
	// If the search reaches the end of the entity list, wrap around to the beginning.
	it = level.Entities().begin();
	for( ; it != level.Entities().end(); ++it)
		if((*it)->IsControlled())
		{
			controlledEntity = *it;
			return;
		}
		
	// If no other controllable entity is found, control remains with the same entity.
}



bool GameData::CanTunnelUp() const
{
	return CanEnterTunnel() && PlayerTile().IsTunnelUp();
}



bool GameData::CanTunnelDown() const
{
	return CanEnterTunnel() && PlayerTile().IsTunnelDown();
}



const Material &GameData::GetMaterial(const TileType &tileType) const
{
	if(tileType.MaterialIndex() >= materials.size())
		throw runtime_error("Unrecognized material index.");
	
	return *materials[tileType.MaterialIndex()];
}



GameData::GameData()
{
}



string GameData::FullPath(const string &path)
{
	if(path.empty())
		throw runtime_error("Error: Empty file path.");
		
	if(*path.begin() == '/')
		return path;
	
	return rootDirectory + path;
}



void GameData::Read(const string &path)
{
	DataFile in(path);
	
	if(!in)
		return;
		
	LevelChanges::GetInstance().Clear();
	
	string tag;
	
	while(in>>tag)
	{
		if(tag == "level")
		{
			SharedPtr<Level> newLevel(new Level(in, tileTypes));
			
			size_t index = static_cast<size_t>(newLevel->Index());
			
			// Insert each new level into the proper slot in the levels vector.
			// If no entry exists yet for that level, make one.
			if(index >= levels.size())
				levels.resize(index + 1);
			
			// If the level with the given index already exists, overwrite only the
			// portions of its data that are redefined in the new level entry.
			if(levels[index])
				levels[index]->Update(*newLevel);
			else
				levels[index] = newLevel;
		}
		else if(tag == "tile")
		{
			SharedPtr<TileType> newTile(new TileType(in));
			
			size_t index = static_cast<size_t>(newTile->Index());
			
			// Insert the new tile type into the proper slot in the tile type vector.
			// Resize the vector if necessary to fit the new entry.
			if(index >= tileTypes.size())
				tileTypes.resize(index + 1);
			
			tileTypes[index] = newTile;
		}
		else if(tag == "material")
		{
			SharedPtr<Material> newMaterial(new Material(in));
			
			size_t index = static_cast<size_t>(newMaterial->Index());
			
			// Insert the new material into the proper slot in the materials vector.
			// Resize the vector if necessary to fit the new entry.
			if(index >= materials.size())
				materials.resize(index + 1);
			
			materials[index] = newMaterial;
		}
		else if(tag == "player")
		{
			in>>Player::GetInstance();
		}
		else if(tag == "image")
		{
			sprites.Add(in, rootDirectory);
		}
		else if(tag == "sound")
		{
			SoundEngine::GetInstance().Add(in, rootDirectory);
		}
	}
}



void GameData::FindPlayer()
{
	player.Reset();
	
	for(currentLevelIndex = 0; currentLevelIndex < levels.size(); ++currentLevelIndex)
	{
		const Level &level = *levels[currentLevelIndex];
		
		for(EntityList::const_iterator it = level.Entities().begin(); it != level.Entities().end(); ++it)
			if((*it)->Name() == "player")
			{
				player = *it;
				controlledEntity = player;
				return;
			}
	}
}



// Find out what tile type the player is on.
const TileType &GameData::PlayerTile() const
{
	return (*levels[currentLevelIndex])(static_cast<int>(player->X()), static_cast<int>(player->Y()));	
}



// Check whether the player is moving slowly enough to enter a tunnel.
// Also returns false if the player is not the actively controlled entity,
// or if the player is not centered on a tile.
bool GameData::CanEnterTunnel() const
{
	if(!player)
		return false;
		
	float fx = fmod(player->X(), 1.0f);
	float fy = fmod(player->Y(), 1.0f);
	float fMin = player->Radius();
	float fMax = 1.0f - fMin;
	
	return (controlledEntity == player) & 
		(player->VelocityX() * player->VelocityX() + player->VelocityY() * player->VelocityY() < .5f) &
		(fx >= fMin) & (fx <= fMax) & (fy >= fMin) & (fy <= fMax);
}



GameData GameData::instance;
