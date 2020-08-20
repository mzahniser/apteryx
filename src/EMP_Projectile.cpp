/* EMP_Projectile.cpp
Michael Zahniser, 10/02/07

Function definitions for the EMP_Projectile class.
*/

#include "EMP_Projectile.h"

#include "LevelChanges.h"
#include "Deactivate_AI.h"
#include "Explosion_Effect.h"



EMP_Projectile::EMP_Projectile(const Entity &source, int effectDuration)
	: Projectile(source), duration(effectDuration)
{
	SetSprite("empProjectile");
}



void EMP_Projectile::OnCollide(Object &object)
{
	if(object.IsEnemy())
	{
		// The EMP can only collide with drones.
		Drone_Entity &drone = reinterpret_cast<Drone_Entity &>(object);
		
		drone.SetEffect(StandardAI::Ptr(new Deactivate_AI(drone)), duration);
		LevelChanges::GetInstance().Add(new Explosion_Effect("empStrike", drone));
	}
	else
	{
		// This should never happen, but to be safe...
		Entity &entity = reinterpret_cast<Entity &>(object);
		entity.SetEffect(StandardAI::Ptr(new Drift_AI(entity)), duration);
		LevelChanges::GetInstance().Add(new Explosion_Effect("empStrike", entity));
	}
		
	LevelChanges::GetInstance().Remove(this);
}



void EMP_Projectile::Die()
{
	LevelChanges::GetInstance().Add(new Explosion_Effect("empStrike", *this));	
}
