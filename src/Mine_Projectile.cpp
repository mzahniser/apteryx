/* Mine_Projectile.cpp
Michael Zahniser, 10/02/07

Function definitions for the Mine_Projectile class.
*/

#include "Mine_Projectile.h"

#include "LevelChanges.h"
#include "Explosion_Effect.h"



Mine_Projectile::Mine_Projectile(const Entity &source)
	: Projectile(source, .3f, 100)
{
	SetSprite("mineProjectile");
}



void Mine_Projectile::OnCollide(Object &object)
{
	AddShields(object, -3.0f);
	
	LevelChanges::GetInstance().Remove(this);
	Die();
}



void Mine_Projectile::Die()
{
	LevelChanges::GetInstance().Add(new Explosion_Effect("mineExplosion", *this));	
}
