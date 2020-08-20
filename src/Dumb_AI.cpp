/* Dumb_AI.cpp
Michael Zahniser, 7/11/07.

Function definitions for the Dumb_AI class.
*/

#include "Dumb_AI.h"

#include <cmath>

using std::fabs;



Dumb_AI::Dumb_AI(Drone_Entity &theDrone)
	: drone(theDrone)
{
}



void Dumb_AI::ApplyWithTarget(const Object &target)
{
	if(drone.IsActive())
		SeekTarget(target, drone.VelocityX(), drone.VelocityY());
	
	drone.SetTargetIsVisible(true);
	
	FirePrimary(drone, drone.IsActive());
}



void Dumb_AI::SeekTarget(const Object &target, float velocityX, float velocityY)
{
	SeekTarget(Speed(drone), target.X(), target.Y(), velocityX, velocityY);
}



void Dumb_AI::SeekTarget(float speed, float targetX, float targetY, float velocityX, float velocityY)
{
	float stopTimeX = fabs(velocityX / speed);
	float stopX = drone.X() + .5f * velocityX * stopTimeX;
	AccelerateX(drone, (stopX < targetX) - (stopX > targetX));
	
	float stopTimeY = fabs(velocityY / speed);
	float stopY = drone.Y() + .5f * velocityY * stopTimeY;
	AccelerateY(drone, (stopY < targetY) - (stopY > targetY));	
}

