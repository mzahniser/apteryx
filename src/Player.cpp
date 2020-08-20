/* Player.cpp
Michael Zahniser, 09/30/07

Function defintions for the Player class.
*/

#include "Player.h"

#include "LevelChanges.h"
#include "GameData.h"
#include "SoundEngine.h"

#include "Laser_Projectile.h"
#include "Mine_Projectile.h"
#include "EMP_Projectile.h"
#include "Virus_Projectile.h"
#include "Fusion_Projectile.h"
#include "Hexagon_Effect.h"
#include "SpyBot_Entity.h"

#include "Repulsor_AI.h"
#include "TractorBeam_AI.h"
#include "EMPBurst_AI.h"


Player &Player::GetInstance()
{
	return instance;
}



void Player::NewPlayer()
{
	shields = 10.0f;
	energy = 10.0f;
	selectedWeapon = NO_WEAPON;
	weapons.clear();
	systems.clear();
}



	
bool Player::AddShields(float delta)
{
	if(delta < 0)
		SoundEngine::GetInstance().Play("hit");
	return AddTo(shields, delta);
}



bool Player::AddEnergy(float delta)
{
	if(energy + delta < 0.0f)
		return false;
	return AddTo(energy, delta);
}



int Player::UpgradeCount(unsigned char upgrade) const
{
	if(upgrade >= SYSTEM)
	{
		upgrade -= SYSTEM;
		
		if(upgrade >= systems.size())
			return 0;
		
		return systems[upgrade];
	}
	else
	{
		if(upgrade >= weapons.size())
			return 0;
		
		return weapons[upgrade];
	}
}



bool Player::AddUpgrade(unsigned char upgrade)
{
	// Upgrade 0 is reserved for "No weapon selected."
	if(!upgrade)
		return false;
	
	if(upgrade >= SYSTEM)
	{
		upgrade -= SYSTEM;
		
		if(upgrade >= systems.size())
			systems.resize(upgrade + 1, 0);
			
		++systems[upgrade];
	}
	else
	{
		if(upgrade >= weapons.size())
			weapons.resize(upgrade + 1, 0);
		
		++weapons[upgrade];
	}
	
	return true;
}



void Player::SelectNextWeapon()
{
	SelectWeapon(selectedWeapon + 1);
}


	
void Player::SelectWeapon(unsigned char weapon)
{
	selectedWeapon = weapon;
	
	while(selectedWeapon < weapons.size() && !weapons[selectedWeapon])
		++selectedWeapon;
		
	if(selectedWeapon >= weapons.size())
		selectedWeapon = 0;
}



// Check whether the primary weapon is ready to fire (reload time elapsed, and
// enough energy to fire). If it is, fire a laser from the given entity.
bool Player::FirePrimary(const Entity &source)
{
	if(reload < RELOAD_TIME || !AddEnergy(-.4))
		return false;
		
	LevelChanges::GetInstance().Add(new Laser_Projectile(source));
	reload -= RELOAD_TIME;
	
	SoundEngine::GetInstance().Play("primary");
	
	return true;
}



// Fire the secondary weapon.
bool Player::FireSecondary(const Entity &source)
{
	LevelChanges &changes = LevelChanges::GetInstance();
	
	if(selectedWeapon == MINE)
		changes.Add(new Mine_Projectile(source));
	else if(selectedWeapon == EMP_SHOT)
		changes.Add(new EMP_Projectile(source, 60));
	else if(selectedWeapon == VIRUS)
		changes.Add(new Virus_Projectile(source));
	else if(selectedWeapon == FUSION_SHOT)
		changes.Add(new Fusion_Projectile(source));
	else if(selectedWeapon == HEXAGON)
		changes.Add(new Hexagon_Effect(source, 100));
	else
		return false;
		
	SoundEngine::GetInstance().Play("secondary");
	
	--weapons[selectedWeapon];
	if(!weapons[selectedWeapon])
		SelectNextWeapon();
	
	return true;
}



// Turn the secondary weapon on or off (continuous effect).
void Player::ApplySecondary(bool isApplied)
{
	Entity &player = *GameData::GetInstance().PlayerEntity();
	if((selectedWeapon == AFTERBURNER) & isApplied & (energy >= .1f) & 
		((player.accelerationX != 0.0f) | (player.accelerationY != 0.0f)))
	{
		player.accelerationX *= 1.5f;
		player.accelerationY *= 1.5f;
		player.speed = 4.5f;
		
		energy -= .1f;
		SoundEngine::GetInstance().PlayLoop("afterburner");
		
		return;
	}
	else
		player.speed = 3.0f;
	
	if(!isApplied)
	{
		weaponEffect.Reset();
		SoundEngine::GetInstance().EndLoop();		
		weaponEffectCost = 0.0f;
		return;
	}
	
	if(!weaponEffect)
	{
		if(selectedWeapon == REPULSOR)
		{
			weaponEffect = GlobalAI::Ptr(new Repulsor_AI(player));
			weaponEffectCost = .1f;
			SoundEngine::GetInstance().PlayLoop("beam");
		}
		else if(selectedWeapon == TRACTOR_BEAM)
		{
			weaponEffect = GlobalAI::Ptr(new TractorBeam_AI(player));
			weaponEffectCost = .1f;
			SoundEngine::GetInstance().PlayLoop("beam");
		}
		else if(selectedWeapon == CLOAK)
		{
			weaponEffect = GlobalAI::Ptr(new GlobalAI());
			weaponEffectCost = .1f;
			SoundEngine::GetInstance().PlayLoop("afterburner");
		}
		else if(selectedWeapon == EMP_BURST)
		{
			weaponEffect = GlobalAI::Ptr(new EMPBurst_AI(100));
			weaponEffectCost = 5.5f;
			SoundEngine::GetInstance().Play("crash");
		}
	}

	if(energy < weaponEffectCost)
	{
		weaponEffect.Reset();
		SoundEngine::GetInstance().EndLoop();		
		weaponEffectCost = 0.0f;
	}
	else
		energy -= weaponEffectCost;
	
	return;
}



int Player::SelectedWeaponSprite() const
{
	return selectedWeapon;
}



void Player::Animate()
{
	if(shields < 0.0f)
		return;
	
	shields += .001f * static_cast<float>(UpgradeCount(SHIELD_GEN));
	if(shields > 10.0f)
		shields = 10.0f;
	
	energy += .001f * static_cast<float>(UpgradeCount(ENERGY_GEN));
	if(energy > 10.0f)
		energy = 10.0f;
	
	if(reload < RELOAD_TIME)
		reload += UpgradeCount(LASER);
}



ostream &operator<<(ostream &out, const Player &player)
{
	out<<"player";
	out<<"\n\tshields "<<player.shields;
	out<<"\n\tenergy "<<player.energy;
	for(int i = 0; i < player.weapons.size(); ++i)
		if(player.weapons[i])
			out<<"\n\tupgrade "<<(i + Player::WEAPON)<<' '<<static_cast<int>(player.weapons[i]);
	for(int i = 0; i < player.systems.size(); ++i)
		if(player.systems[i])
			out<<"\n\tupgrade "<<(i + Player::SYSTEM)<<' '<<static_cast<int>(player.systems[i]);
	if(player.selectedWeapon)
		out<<"\n\tselected "<<static_cast<int>(player.selectedWeapon);
	out<<"\nend\n\n";
	
	return out;
}



istream &operator>>(istream &in, Player &player)
{
	player.NewPlayer();
	
	string tag;
	in>>tag;
	
	while(tag != "end")
	{
		if(tag == "shields")
			in>>player.shields;
		else if(tag == "energy")
			in>>player.energy;
		else if(tag == "upgrade")
		{
			int type;
			in>>type;
			
			int count;
			in>>count;
			
			player.AddUpgrade(type, count);
		}
		else if(tag == "selected")
		{
			int selected;
			in>>selected;
			
			player.selectedWeapon = static_cast<unsigned char>(selected);
		}
	
		in>>tag;
	}
	
	return in;
}



Player::Player()
	: selectedWeapon(NO_WEAPON), shields(10.0f), energy(10.0f)
{
}



void Player::AddUpgrade(unsigned char upgrade, unsigned char count)
{
	// Make sure the vector entry for this upgrade exists.
	if(AddUpgrade(upgrade))
	{
		if(upgrade >= SYSTEM)
			systems[upgrade - SYSTEM] += count - 1;
		else
			weapons[upgrade] += count - 1;
	}
}



bool Player::AddTo(float &value, float delta)
{
	// Take refills only if at least 20% of the total value is useful.
	// This never evaluates to true if the delta is negative (damage).
	if((delta == 0.0f) | (10.0f - value < .2f * delta))
		return false;
		
	value += delta;
	if(value > 10.0f)
		value = 10.0f;
		
	return true;
}




Player Player::instance;
