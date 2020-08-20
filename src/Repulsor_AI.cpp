/* Repulsor_AI.cpp
Michael Zahniser, 7/11/07.

Function definitions for the Repulsor_AI class.
*/

#include "Repulsor_AI.h"



Repulsor_AI::Repulsor_AI(Object &beamSource)
	: source(beamSource)
{
}


	
void Repulsor_AI::ApplyTo(Entity &target) const
{
	AccelerateX(target, (target.X() > source.X()) - (target.X() < source.X()));
	AccelerateY(target, (target.Y() > source.Y()) - (target.Y() < source.Y()));
}
