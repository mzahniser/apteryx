/* TractorBeam_AI.cpp
Michael Zahniser, 7/11/07.

Function definitions for the TractorBeam_AI class.
*/

#include "TractorBeam_AI.h"

#include <cmath>

using std::fabs;



TractorBeam_AI::TractorBeam_AI(Object &beamSource)
	: source(beamSource)
{
}


	
void TractorBeam_AI::ApplyTo(Entity &target) const
{
	AccelerateX(target, (target.VelocityX() < source.VelocityX()) - (target.VelocityX() > source.VelocityX()));
	AccelerateY(target, (target.VelocityY() < source.VelocityY()) - (target.VelocityY() > source.VelocityY()));
}
