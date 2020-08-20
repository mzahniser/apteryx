/*
 *  Upgrade_Entity.cpp
 *  Apteryx
 *
 *  Created by Michael Zahniser on 9/30/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Upgrade_Entity.h"
#include "SoundEngine.h"

#include "LevelChanges.h"
#include "Player.h"



Upgrade_Entity::Upgrade_Entity(int variant, float x, float y)
	: Entity(x, y, .3f, IS_UPGRADE | AFFECTS_PLAYER), type(variant), slowFrame(0)
{
	SetSprite("upgrades", 3 * type);
		
	SetName(NAME[type]);
}



void Upgrade_Entity::Animate()
{
	// Animate slowly.
	++slowFrame;
	slowFrame *= (slowFrame != 12);
	SetSpriteFrame(type * 3 + slowFrame / 4);
}



void Upgrade_Entity::Register(Entity::ConstructorMap &constructors)
{
	for(int i = 0; i < TYPES; ++i)
		constructors[NAME[i]] = VariantConstructor(&Constructor, i);
}



Entity::Ptr Upgrade_Entity::Constructor(int variant, float x, float y)
{
	return Entity::Ptr(new Upgrade_Entity(variant, x, y));
}

	

void Upgrade_Entity::OnCollide(Object &object)
{
	Player &player = Player::GetInstance();
	
	if(player.AddShields(SHIELDS[type]) | player.AddEnergy(ENERGY[type]) | player.AddUpgrade(UPGRADE[type]))
	{
		LevelChanges::GetInstance().Remove(this);
		
		if(SHIELDS[type])
			SoundEngine::GetInstance().Play("shieldUp");
		else if(ENERGY[type])
			SoundEngine::GetInstance().Play("energyUp");
		else
			SoundEngine::GetInstance().Play("secondary");
	}
}



const string Upgrade_Entity::NAME[TYPES] = {
	"energySmall",
	"energyFull",
	"shieldSmall",
	"shieldFull",
	"laser",
	"mine",
	"emp",
	"virus",
	"fusion",
	"afterburner",
	"repulsor",
	"tractor",
	"cloak",
	"empBurst",
	"hexagon",
	"shieldGenerator",
	"energyGenerator",
	"radar"	
};



const unsigned char Upgrade_Entity::UPGRADE[TYPES] = {
	Player::NO_WEAPON,
	Player::NO_WEAPON,
	Player::NO_WEAPON,
	Player::NO_WEAPON,
	Player::LASER,
	Player::MINE,
	Player::EMP_SHOT,
	Player::VIRUS,
	Player::FUSION_SHOT,
	Player::AFTERBURNER,
	Player::REPULSOR,
	Player::TRACTOR_BEAM,
	Player::CLOAK,
	Player::EMP_BURST,
	Player::HEXAGON,
	Player::SHIELD_GEN,
	Player::ENERGY_GEN,
	Player::RADAR
};

const float Upgrade_Entity::SHIELDS[TYPES] = {
	0.0f, 0.0f, 2.0f, 10.0f, 
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

const float Upgrade_Entity::ENERGY[TYPES] = {
	2.0f, 10.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
