/* AI.h
Michael Zahniser, 07/10/07

Function definitions for the AI base class.
*/

#include "AI.h"



void AI::AccelerateX(Object &object, int direction)
{
	object.accelerationX = object.speed * static_cast<float>((direction > 0) - (direction < 0));
}



void AI::AccelerateY(Object &object, int direction)
{
	object.accelerationY = object.speed * static_cast<float>((direction > 0) - (direction < 0));
}



void AI::AccelerateX(Object &object, float direction)
{
	object.accelerationX = (direction < object.speed) ? (direction > -object.speed) ? 
		direction : -object.speed : object.speed;
}



void AI::AccelerateY(Object &object, float direction)
{
	object.accelerationY = (direction < object.speed) ? (direction > -object.speed) ? 
		direction : -object.speed : object.speed;
}



void AI::FirePrimary(Object &object, bool shouldFire)
{
	object.firePrimary = shouldFire;
}



void AI::ApplySecondary(Object &object, bool isApplied)
{
	object.fireSecondary |= (isApplied & !object.applySecondary);
	object.applySecondary = isApplied;
}



// "Cheater" option to set velocity directly.
void AI::CheatVelocityX(Object &object, float velocity)
{
	object.velocityX = velocity;
}



void AI::CheatVelocityY(Object &object, float velocity)
{
	object.velocityY = velocity;
}
