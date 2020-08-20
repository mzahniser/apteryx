/* Missile.cpp
Michael Zahniser, 09/29/07

Function definitions for the Missile class.
*/

#include "Missile.h"

#include "LevelChanges.h"

#include <cmath>

using std::atan2;



Missile::Missile(const Entity &source, float velocityFraction, int lifetime)
	: Projectile(source, velocityFraction, lifetime)
{
	// Set the sprite frame based on the angle of motion.
	float angle = (8.0f / 3.14159265358979f) * atan2(-VelocityY(), VelocityX()) + .5f;
	if(angle < 0.0f)
		angle += 16.0f;
	SetSpriteFrame(static_cast<int>(angle));
}



Missile::Missile(unsigned char typeFlags, const Entity &source, int lifetime)
	: Projectile(typeFlags, source, lifetime)
{
	// Set the sprite frame based on the angle of motion.
	float angle = (8.0f / 3.14159265358979f) * atan2(-VelocityY(), VelocityX()) + .5f;
	if(angle < 0.0f)
		angle += 16.0f;
	SetSpriteFrame(static_cast<int>(angle));	
}


	
void Missile::Animate()
{
	Projectile::Animate();

	// Set the sprite frame based on the angle of motion.
	float angle = (8.0f / 3.14159265358979f) * atan2(-VelocityY(), VelocityX()) + .5f;
	if(angle < 0.0f)
		angle += 16.0f;
	SetSpriteFrame(static_cast<int>(angle));	
}
