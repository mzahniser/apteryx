/* Laser_Projectile.cpp
Michael Zahniser, 09/29/07

Function definitions for the Laser_Projectile class.
*/

#include "Laser_Projectile.h"

#include "LevelChanges.h"
#include "Shield_Effect.h"



Laser_Projectile::Laser_Projectile(const Entity &source)
	: Projectile(source)
{
	SetSprite(source.IsPlayer() ? "playerLaser" : "enemyLaser");
}



void Laser_Projectile::OnCollide(Object &object)
{
	AddShields(object, -1.0f);
	
	LevelChanges &changes = LevelChanges::GetInstance();
	
	if(object.IsPlayer())
		changes.Add(new Shield_Effect(object, X() - object.X(), Y() - object.Y()));
	
	changes.Remove(this);
}
