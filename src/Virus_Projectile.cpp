/* Virus_Projectile.cpp
Michael Zahniser, 10/02/07

Function definitions for the Virus_Projectile class.
*/

#include "Virus_Projectile.h"

#include "LevelChanges.h"
#include "Drone_Entity.h"
#include "ControlledDrone_Entity.h"
#include "Explosion_Effect.h"



Virus_Projectile::Virus_Projectile(const Entity &source)
	: Missile(source, 3.0f, 60)
{
	SetSprite("virusProjectile");
}



void Virus_Projectile::OnCollide(Object &object)
{
	// If this isn't an enemy, bail out.
	if(!object.IsEnemy())
		return;
	
	Drone_Entity &entity = reinterpret_cast<Drone_Entity &>(object);
	LevelChanges &changes = LevelChanges::GetInstance();

	changes.Add(new ControlledDrone_Entity(entity));
	changes.Add(new Explosion_Effect("virusStrike", entity));
	changes.Remove(&entity);
	changes.Remove(this);
}



void Virus_Projectile::Die()
{
	LevelChanges::GetInstance().Add(new Explosion_Effect("virusStrike", *this));
}
