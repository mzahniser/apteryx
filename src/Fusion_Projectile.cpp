/* Fusion_Projectile.cpp
Michael Zahniser, 10/02/07

Function definitions for the Fusion_Projectile class.
*/

#include "Fusion_Projectile.h"

#include "LevelChanges.h"

#include "Explosion_Effect.h"



Fusion_Projectile::Fusion_Projectile(const Entity &source)
	: Projectile(source)
{
	SetSprite("fusionProjectile");
}



Fusion_Projectile::Fusion_Projectile(const Entity &splitFrom, int typeFlags)
	: Projectile(typeFlags, splitFrom)
{
	SetSprite("fusionProjectile");
}



void Fusion_Projectile::Animate()
{
	Projectile::Animate();
	
	if(!SpriteFrame())
		LevelChanges::GetInstance().Add(new Explosion_Effect("fusionExplosion", *this, 0.2f));		
}



void Fusion_Projectile::OnCollide(Object &object)
{
	if(AddShields(object, -3.0f))
	{
		Fusion_Projectile *left = new Fusion_Projectile(*this, AFFECTS_ENEMY);
		left->SetVelocity(VelocityY(), -VelocityX());
		LevelChanges::GetInstance().Add(left);
		
		Fusion_Projectile *right = new Fusion_Projectile(*this, AFFECTS_ENEMY);
		right->SetVelocity(-VelocityY(), VelocityX());
		LevelChanges::GetInstance().Add(right);
		
		LevelChanges::GetInstance().Add(new Explosion_Effect("fusionExplosion", *this));
	}
}



void Fusion_Projectile::Die()
{
	LevelChanges::GetInstance().Add(new Explosion_Effect("fusionExplosion", *this));	
}
