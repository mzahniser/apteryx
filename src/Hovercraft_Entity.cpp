/* Hovercraft_Entity.cpp
Michael Zahniser, 09/26/07

Function definitions for the Hovercraft_Entity class.
*/

#include "Hovercraft_Entity.h"

#include "Drift_AI.h"
#include "LevelChanges.h"
#include "Player.h"
#include "Laser_Projectile.h"
#include "Explosion_Effect.h"
#include "Dead_Entity.h"
#include "Shield_Effect.h"



Hovercraft_Entity::Hovercraft_Entity(float x, float y)
	: Entity(x, y, .32f, IS_PLAYER | AFFECTS_ENEMY), slowFrame(0)
{
	SetAI(StandardAI::Ptr(new Drift_AI(*this)));
	
	SetSpeed(3.0f);
	
	SetName("player");
	SetSprite("player", 4 * 3);
}



void Hovercraft_Entity::Animate()
{
	if(FirePrimary())
		Player::GetInstance().FirePrimary(*this);
	if(FireSecondary())
		Player::GetInstance().FireSecondary(*this);
	Player::GetInstance().ApplySecondary(ApplySecondary());
	
	if(++slowFrame == 12)
		slowFrame = 0;

	int directionX = (AccelerationX() > 0.0f) - (AccelerationX() < 0.0f);
	int directionY = (AccelerationY() > 0.0f) - (AccelerationY() < 0.0f);
	
	SetSpriteFrame(3 * (4 - directionY * 3 - directionX) + slowFrame / 4);	
}



void Hovercraft_Entity::Register(Entity::ConstructorMap &constructors)
{
	constructors["player"] = Entity::VariantConstructor(&Constructor, 0);
}



Entity::Ptr Hovercraft_Entity::Constructor(int variant, float x, float y)
{
	return Entity::Ptr(new Hovercraft_Entity(x, y));
}



void Hovercraft_Entity::OnCollide(Object &object)
{
	LevelChanges::GetInstance().Add(new Shield_Effect(*this, object.X() - X(), object.Y() - Y()));
}



// Add shields. Returns false if shields can't be added (e.g. already full).
bool Hovercraft_Entity::AddShields(float amount)
{
	Player &player = Player::GetInstance();
	
	if(player.Shields() < 0.0f)
		return false;
	
	bool wereAdded = player.AddShields(amount);
	
	if(player.Shields() < 0.0f)
	{
		LevelChanges &changes = LevelChanges::GetInstance();

		changes.Remove(this);
		changes.Add(new Dead_Entity(*this, 5));
		changes.Add(new Explosion_Effect("explosion", *this));	
	}
	
	return wereAdded;
}

