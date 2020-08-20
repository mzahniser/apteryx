/* Explosion_Effect.cpp
Michael Zahniser, 09/29/07

Function definitions for the ExplosionEffect class.
*/

#include "Explosion_Effect.h"

#include "LevelChanges.h"

#include <cstdlib>


using std::rand;



Explosion_Effect::Explosion_Effect(const string &spriteName, const Entity &entity, float relativeVelocity)
	: Entity(entity, relativeVelocity)
{
	SetSprite(spriteName);
	if(spriteName == "explosion")
		SetSpriteFrame(8 * (rand() % 3));
}



// Get the index of the sprite to use for this object, given the current time.
void Explosion_Effect::Animate()
{
	SetSpriteFrame(SpriteFrame() + 1);
	if(!(SpriteFrame() & 7))
		LevelChanges::GetInstance().Remove(this);
}
