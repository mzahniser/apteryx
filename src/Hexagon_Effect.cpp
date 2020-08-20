/* Hexagon_Effect.cpp
Michael Zahniser, 10/03/07

Function definitions for the Hexagon_Effect class.
*/

#include "Hexagon_Effect.h"

#include "LevelChanges.h"



Hexagon_Effect::Hexagon_Effect(const Entity &source, int lifetime)
	: Entity(source, 1.0f, .8f, IS_UPGRADE | AFFECTS_ENEMY), life(lifetime),
	velocityX(source.VelocityX()), velocityY(source.VelocityY())
{
	SetSprite("upgrades", 3 * 14);
}



void Hexagon_Effect::Animate()
{
	if(!--life)
		LevelChanges::GetInstance().Remove(this);
}



void Hexagon_Effect::OnCollide(Object &object)
{
	SetVelocity(velocityX, velocityY);
}
