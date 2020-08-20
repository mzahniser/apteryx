/* TractorBeam_AI.h
Michael Zahniser, 7/11/07.

Header file for the TractorBeam_AI class, which defines how all drones should behave
when the tractor beam is activated.
*/

#ifndef __MZ_TRACTOR_BEAM_AI_H__
#define __MZ_TRACTOR_BEAM_AI_H__


#include "GlobalAI.h"

#include "Object.h"



class TractorBeam_AI : public GlobalAI {
public:
	TractorBeam_AI(Object &beamSource);
	
	virtual void ApplyTo(Entity &target) const;

	
private:
	Object &source;
};

#endif