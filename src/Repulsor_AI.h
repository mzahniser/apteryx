/* Repulsor_AI.h
Michael Zahniser, 7/11/07.

Header file for the Repulsor_AI class, which defines how all drones should behave
when the repulsor beam is activated.
*/

#ifndef __MZ_REPULSOR_AI_H__
#define __MZ_REPULSOR_AI_H__


#include "GlobalAI.h"

#include "Object.h"



class Repulsor_AI : public GlobalAI {
public:
	Repulsor_AI(Object &beamSource);
	
	virtual void ApplyTo(Entity &target) const;

	
private:
	Object &source;
};

#endif