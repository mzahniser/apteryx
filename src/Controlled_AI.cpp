/* Controlled_AI.cpp
Michael Zahniser, 9/26/07

Function definitions for the Controlled_AI class.
*/

#include "Controlled_AI.h"



Controlled_AI::Controlled_AI()
	: left(false), up(false), right(false), down(false), 
	firePrimary(false), fireSecondary(false), cheat(false)
{
}



void Controlled_AI::ApplyTo(Entity &target) const
{
	if(cheat)
	{
		CheatVelocityX(target, velocityX);
		CheatVelocityY(target, velocityY);
	}
	else
	{
		int directionX = right - left;
		int directionY = down - up;
		
		AccelerateX(target, directionX);
		AccelerateY(target, directionY);
	}

	FirePrimary(target, firePrimary);
	ApplySecondary(target, fireSecondary);
}



void Controlled_AI::KeyDown(Direction direction)
{
	left |= (direction == LEFT);
	up |= (direction == UP);
	right |= (direction == RIGHT);
	down |= (direction == DOWN);
	
	firePrimary |= (direction == PRIMARY);
	fireSecondary |= (direction == SECONDARY);
	
	cheat = false;
}



void Controlled_AI::KeyUp(Direction direction)
{
	left &= ~(direction == LEFT);
	up &= ~(direction == UP);
	right &= ~(direction == RIGHT);
	down &= ~(direction == DOWN);
	
	firePrimary &= ~(direction == PRIMARY);
	fireSecondary &= ~(direction == SECONDARY);

	cheat = false;
}



void Controlled_AI::CheatSetVelocity(float vx, float vy)
{
	cheat = true;
	velocityX = vx;
	velocityY = vy;
}
