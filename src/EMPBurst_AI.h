/* EMPBurst_AI.h
Michael Zahniser, 10/03/07.

Header file for the EMPBurst_AI class, which defines how all drones should behave
when the repulsor beam is activated.
*/

#ifndef __MZ_EMP_BURST_AI_H__
#define __MZ_EMP_BURST_AI_H__


#include "GlobalAI.h"

#include "Object.h"



class EMPBurst_AI : public GlobalAI {
public:
	EMPBurst_AI(int effectDuration);
	
	virtual void ApplyTo(Entity &target) const;
	
	
private:
	int duration;
};

#endif