/* Projectile.cpp
Michael Zahniser, 09/29/07

Function definitions for the Projectile class.
*/

#include "Projectile.h"

#include "LevelChanges.h"



Projectile::Projectile(const Entity &source, float velocityFraction, int lifetime)
	: Entity(source, velocityFraction, .1f, 
		(source.IsEnemy() ? AFFECTS_PLAYER : AFFECTS_ENEMY)),
	life(lifetime), frames(8)
{
}



Projectile::Projectile(unsigned char typeFlags, const Entity &source, int lifetime)
	: Entity(source, 1.0f, .1f, typeFlags), life(lifetime), frames(8)
{
}


	
void Projectile::Animate()
{
	SetSpriteFrame((SpriteFrame() + 1) % frames);
	
	if(!--life)
	{
		LevelChanges::GetInstance().Remove(this);
		Die();
	}
}



void Projectile::OnBounce(const TileType &tileType)
{
	// Projectiles bounce off force fields.
	if(!tileType.FieldTag() | tileType.IsSwitch())
	{
		LevelChanges::GetInstance().Remove(this);
		Die();
	}
}