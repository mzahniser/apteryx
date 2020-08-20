/* Shield_Effect.cpp
Michael Zahniser, 09/29/07

Function definitions for the ExplosionEffect class.
*/

#include "Shield_Effect.h"

#include "LevelChanges.h"



Shield_Effect::Shield_Effect(const Object &object, float dx, float dy)
	: Entity(object, 1.0f)
{
	bool a = (dy * 2.0 > dx);
	bool b = (dy * 2.0 < -dx) ^ a;
	bool c = (dy < -2.0 * dx) ^ a;
	bool d = (dy > 2.0 * dx) ^ a;
	
	int direction = 4 * a + b + c + d;
	
	SetSprite("shieldStrike", 3 * direction);
}



// Get the index of the sprite to use for this object, given the current time.
void Shield_Effect::Animate()
{
	int nextFrame = SpriteFrame() + 1;
	if(nextFrame % 3)
		SetSpriteFrame(nextFrame);
	else
		LevelChanges::GetInstance().Remove(this);
}
