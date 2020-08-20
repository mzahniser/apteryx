/* Mindful_AI.cpp
Michael Zahniser, 10/22/07

Function definitions for the Mindful_AI class.
*/

#include "Mindful_AI.h"



Mindful_AI::Mindful_AI(Drone_Entity &theDrone)
	: Deactivate_AI(theDrone), Dumb_AI(theDrone)
{
}



void Mindful_AI::Apply()
{
	Deactivate_AI::drone.SetTargetIsVisible(false);
	
	if(Deactivate_AI::drone.LastTargetX() < 0.0f)
		Deactivate_AI::Apply();
	else
	{
		float dx = Deactivate_AI::drone.X() - Deactivate_AI::drone.LastTargetX();
		float dy = Deactivate_AI::drone.Y() - Deactivate_AI::drone.LastTargetY();
		
		if(dx * dx + dy * dy < 1.0f)
			Deactivate_AI::Apply();
		else
		{
			Deactivate_AI::drone.SetTargetIsVisible(true);
			SeekTarget(Deactivate_AI::Speed(Deactivate_AI::drone), 
				Deactivate_AI::drone.LastTargetX(), Deactivate_AI::drone.LastTargetY(), 
				Deactivate_AI::drone.VelocityX(), Deactivate_AI::drone.VelocityY());
		}
	}
}
