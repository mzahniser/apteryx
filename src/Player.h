/* Player.h
Michael Zahniser, 09/30/07

Header file for the Player class, which stores all the player statistics.
This class is a singleton that is accessible from anywhere.
*/

#ifndef __MZ_PLAYER_H__
#define __MZ_PLAYER_H__


#include "Entity.h"

#include "GlobalAI.h"

#include <vector>
#include <ostream>
#include <istream>

using std::vector;
using std::ostream;
using std::istream;



class Player {
public:
	static Player &GetInstance();
	
	void NewPlayer();
	
	float Shields() const { return shields; }
	bool AddShields(float delta);
	
	float Energy() const { return energy; }
	bool AddEnergy(float delta);
	
	int UpgradeCount(unsigned char upgrade) const;
	bool AddUpgrade(unsigned char upgrade);
	
	void SelectNextWeapon();	
	void SelectWeapon(unsigned char weapon);
	int SelectedWeapon() const { return selectedWeapon; }
	int SelectedWeaponSprite() const;

	// Check whether the primary weapon is ready to fire (reload time elapsed, and
	// enough energy to fire). If it is, fire a laser from the given entity.
	bool FirePrimary(const Entity &source);
	// Fire the secondary weapon.
	bool FireSecondary(const Entity &source);
	// Turn the secondary weapon on or off (continuous effect).
	void ApplySecondary(bool isApplied);
	const GlobalAI::Ptr &WeaponEffect() const { return weaponEffect; }
	
	void Animate();
	
	friend ostream &operator<<(ostream &out, const Player &player);
	friend istream &operator>>(istream &in, Player &player);
	
	
public:
	// Weapons and systems:
	static const unsigned char WEAPON = 0;
	static const unsigned char SYSTEM = 128;
	
	static const unsigned char NO_WEAPON = WEAPON + 0;
	static const unsigned char MINE = WEAPON + 1;
	static const unsigned char EMP_SHOT = WEAPON + 2;
	static const unsigned char VIRUS = WEAPON + 3;
	static const unsigned char FUSION_SHOT = WEAPON + 4;
	static const unsigned char AFTERBURNER = WEAPON + 5;
	static const unsigned char REPULSOR = WEAPON + 6;
	static const unsigned char TRACTOR_BEAM = WEAPON + 7;
	static const unsigned char CLOAK = WEAPON + 8;
	static const unsigned char EMP_BURST = WEAPON + 9;
	static const unsigned char HEXAGON = WEAPON + 10;
	
	static const unsigned char LASER = SYSTEM + 0;
	static const unsigned char SHIELD_GEN = SYSTEM + 1;
	static const unsigned char ENERGY_GEN = SYSTEM + 2;
	static const unsigned char RADAR = SYSTEM + 3;
	
	
private:
	Player();
	
	void AddUpgrade(unsigned char upgrade, unsigned char count);

	// Do not allow copying.
	Player(const Player &);
	Player &operator=(const Player &);
	
	
private:
	static bool AddTo(float &value, float delta);
	
	
private:
	vector<unsigned char> weapons;
	vector<unsigned char> systems;
	
	float shields;
	float energy;
	
	unsigned char selectedWeapon;
	
	int reload;
	
	GlobalAI::Ptr weaponEffect;
	float weaponEffectCost;
	
	
private:
	static const int RELOAD_TIME = 17;


private:
	static Player instance;
};

#endif