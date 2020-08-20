/* Smart_AI.h
Michael Zahniser, 7/11/07.

Header file for the Smart_AI class, defining a drone AI that moves toward the
player's current position but does not take the player's velocity into account.
*/

#ifndef __MZ_SMART_AI_H__
#define __MZ_SMART_AI_H__


#include "Dumb_AI.h"

#include "Drone_Entity.h"



class Smart_AI : public Dumb_AI {
public:
	Smart_AI(Drone_Entity &theDrone);
	
	virtual void ApplyWithTarget(const Object &target);
	
	
protected:
	void SeekSmart(const Object &target);
};

#endif