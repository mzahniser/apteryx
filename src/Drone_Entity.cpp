/* Drone_Entity.cpp
Michael Zahniser, 7/10/07.

Functional definitions for the Drone_Entity class.
*/

#include "Drone_Entity.h"

#include "Deactivate_AI.h"
#include "Dumb_AI.h"
#include "Smart_AI.h"
#include "Mindful_AI.h"
#include "LevelChanges.h"
#include "Dead_Entity.h"
#include "Explosion_Effect.h"
#include "Laser_Projectile.h"
#include "SoundEngine.h"

#include <limits>

using std::numeric_limits;



Drone_Entity::Drone_Entity(int variant, float x, float y)
	: Entity(x, y, .33, IS_ENEMY | AFFECTS_PLAYER | AFFECTS_ENEMY | AFFECTS_DEAD),
	reloadTime(255), reload(0),
	shields(2.5f),
	isActive(false),
	targetIsVisible(false),
	droneType(variant)
{
	if(droneType == 5)
		SetAI(StandardAI::Ptr(new Mindful_AI(*this)), TargetedAI::Ptr(new Smart_AI(*this)));
	else if((droneType != 0) & (droneType != 2))
		SetAI(StandardAI::Ptr(new Deactivate_AI(*this)), TargetedAI::Ptr(new Smart_AI(*this)));
	else
		SetAI(StandardAI::Ptr(new Deactivate_AI(*this)), TargetedAI::Ptr(new Dumb_AI(*this)));
	
	if((droneType == 2) | (droneType == 3))
		reloadTime = 4;
		
	if(droneType == 4)
	{
		SetSpeed(4.8f);
		shields = numeric_limits<float>::infinity();
	}
	else if(droneType == 5)
	{
		SetSpeed(2.0f);
	}
	else
	{
		SetSpeed(3.0f);
	}
	
	SetName(NAME[variant]);
	SetSprite(NAME[variant]);
}



void Drone_Entity::Animate()
{
	if(shields < 0.0f)
		return;

	int spriteIndex = SpriteFrame();
	
	if(targetIsVisible)
	{
		if(!spriteIndex)
			SoundEngine::GetInstance().Play("activate", X(), Y());
		++spriteIndex;
	}
	else if(spriteIndex)
		--spriteIndex;
	
	if(spriteIndex >= 18)
		spriteIndex = 9;
		
	isActive = targetIsVisible & (spriteIndex >= 9);
	
	SetSpriteFrame(spriteIndex);
	
	if((FirePrimary() & reloadTime != 255) && ++reload >= reloadTime)
	{
		LevelChanges::GetInstance().Add(new Laser_Projectile(*this));
		SoundEngine::GetInstance().Play("primary", X(), Y());
		reload = 0;
	}
}



void Drone_Entity::Register(Entity::ConstructorMap &constructors)
{
	for(int i = 0; i < TYPES; ++i)
		constructors[NAME[i]] = VariantConstructor(&Constructor, i);
}



Entity::Ptr Drone_Entity::Constructor(int variant, float x, float y)
{
	return Entity::Ptr(new Drone_Entity(variant, x, y));
}



void Drone_Entity::OnCollide(Object &object)
{
	Object::AddShields(object, -7.5f);
	
	AddShields(-7.5f);
	
	if(shields >= 0.0f)
		SoundEngine::GetInstance().Play("metal", X(), Y());
}



void Drone_Entity::OnBounce(const TileType &tileType)
{
	if(LevelChanges::GetInstance().Trigger(tileType))
	{
		shields = -1.0f;
		Die();
	}
}



// Add shields. Returns false if shields can't be added (e.g. already full).
bool Drone_Entity::AddShields(float amount)
{
	// You can only die once.
	if((shields < 0.0f) | (shields == numeric_limits<float>::infinity()))
		return false;
	
	shields += amount;
	if(shields < 0.0f)
		Die();
		
	return true;
}



void Drone_Entity::Die()
{
	LevelChanges &changes = LevelChanges::GetInstance();

	changes.Remove(this);
	changes.Add(new Dead_Entity(*this, droneType));
	changes.Add(new Explosion_Effect("explosion", *this));
	
	SoundEngine::GetInstance().Play("explode", X(), Y());
}



const string Drone_Entity::NAME[TYPES] = {
	"droneRed",
	"droneYellow",
	"droneGreen",
	"droneBlue",
	"droneGrey",
	"hunter"
};

