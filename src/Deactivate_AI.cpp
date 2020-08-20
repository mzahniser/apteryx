/* Deactivate_AI.cpp
Michael Zahniser, 7/11/07.

Function definitions for the Deactivate_AI class.
*/


#include "Deactivate_AI.h"

#include "GameData.h"



Deactivate_AI::Deactivate_AI(Drone_Entity &theDrone)
	: Drift_AI(theDrone), drone(theDrone)
{
}



void Deactivate_AI::Apply()
{
	Drift_AI::Apply();
	
	drone.SetTargetIsVisible(false);
	
	FirePrimary(drone, false);
}
