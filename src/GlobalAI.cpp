/* GlobalAI.cpp
Michael Zahniser, 07/10/07

Function definitions for the GlobalAI class.
*/

#include "GlobalAI.h"



void GlobalAI::ApplyTo(Entity &target) const
{
	AccelerateX(target, 0);
	AccelerateY(target, 0);
	
	FirePrimary(target, false);
	ApplySecondary(target, false);
}
