/* Smart_AI.cpp
Michael Zahniser, 7/11/07.

Function definitions for the Smart_AI class.
*/

#include "Smart_AI.h"

#include <cmath>

using std::fabs;



Smart_AI::Smart_AI(Drone_Entity &theDrone)
	: Dumb_AI(theDrone)
{
}



void Smart_AI::ApplyWithTarget(const Object &target)
{
	if(drone.IsActive())
	{
		// If I am approximately in the target's line of motion and the target
		// is moving towards me, move towards the target using the dumb AI.
		float dx = drone.X() - target.X();
		float dy = drone.Y() - target.Y();
		
		float dDotD = dx * dx + dy * dy;
		float vDotV = target.VelocityX() * target.VelocityX() + target.VelocityY() * target.VelocityY();
		
		float dot = dx * target.VelocityX() + dy * target.VelocityY();
		float angle = dot * dot / (dDotD * vDotV);
		// If the player is moving straight towards the drone,
		// apply the simple AI. Otherwise, apply the smart AI.
		if((angle > .9) & (dot > 0.0f))
			SeekTarget(target, drone.VelocityX(), drone.VelocityY());
		else
			SeekSmart(target);
	}
	
	drone.SetTargetIsVisible(true);
	
	FirePrimary(drone, drone.IsActive());
}



void Smart_AI::SeekSmart(const Object &target)
{
	float velocityX = (drone.VelocityX() - target.VelocityX());
	float velocityY = (drone.VelocityY() - target.VelocityY());
	
	SeekTarget(target, velocityX, velocityY);
}
