/* GameData.h
Michael Zahniser, 09/20/07

Header file for the GameData class.

NOTES:

For a new game, only the "entities.txt" file must be read. The contents of
"data.txt" is not altered during the game.

When loading a saved game, the program should first start a new game, then
load the game updates in the saved game file. This means that the saved
game file need only contain entries for levels the user has visited.

(Or, is it simpler to just have every saved game contain all the levels? We
don't save much disk space otherwise (all entities combined are only 16K)
and this simplifies the load saved game process. This also makes saving
simpler, because we don't need to track how many levels the user had altered
or tell the GameData how many levels to save.)

TODO: Add a "ship" block to the data file. Each ship that is read should
simply override the previous data.
*/

#ifndef __MZ_GAME_DATA_H__
#define __MZ_GAME_DATA_H__


#include "Level.h"
#include "Entity.h"
#include "TileType.h"
#include "Material.h"
#include "SpriteEngine.h"
#include "SharedPtr.h"

#include <vector>
#include <string>

using std::vector;
using std::string;



class GameData {
public:
	static GameData &GetInstance();
	
	void Initialize(const string &resourcePath);
	void NewGame();
	void LoadGame(const string &path);
	void SaveGame(const string &path);
	void ExportMap(const string &path);
	
	int CurrentLevelIndex() const { return currentLevelIndex; }
	// Change the current level, and move the player to the same position, in the new level.
	bool SetCurrentLevel(int index);
	const SharedPtr<Level> &CurrentLevel() const { return levels[currentLevelIndex]; }
	SharedPtr<Level> &CurrentLevel() { return levels[currentLevelIndex]; }

	Entity::Ptr &PlayerEntity() { return player; }
	Entity::Ptr &ControlledEntity() { return controlledEntity; }
	void SwitchControl();
	
	bool CanTunnelUp() const;
	bool CanTunnelDown() const;

	const Material &GetMaterial(const TileType &tileType) const;
	
	const Sprite &GetSprite(const string &name, int frame = 0) const { return sprites.GetSprite(name, frame); }
	const Sprite &GetSprite(int spriteIndex, int frame = 0) const { return sprites.GetSprite(spriteIndex, frame); }
	int SpriteIndex(const string &name) const { return sprites.SpriteIndex(name); }
	
	
private:
	// Private constructor, for the single instance.
	GameData();

	string FullPath(const string &path);
	void Read(const string &path);

	void FindPlayer();
	// Find out what tile type the player is on.
	const TileType &PlayerTile() const;
	// Check whether the player is moving slowly enough to enter a tunnel.
	// Also returns false if the player is not the actively controlled entity.
	bool CanEnterTunnel() const;
	
	
private:
	string rootDirectory;
	
	int currentLevelIndex;
	Entity::Ptr player;
	Entity::Ptr controlledEntity;
	
	vector< SharedPtr<Level> > levels;
	vector< SharedPtr<TileType> > tileTypes;
	vector< SharedPtr<Material> > materials;
	
	SpriteEngine sprites;
	
	
private:
	static GameData instance;
	
	
private:
	// Do not allow copying.
	GameData(const GameData &);
	GameData &operator=(const GameData &);
};

#endif