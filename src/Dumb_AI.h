/* Dumb_AI.h
Michael Zahniser, 7/11/07.

Header file for the Dumb_AI class, defining a drone AI that moves toward the
player's current position but does not take the player's velocity into account.
*/

#ifndef __MZ_DUMB_AI_H__
#define __MZ_DUMB_AI_H__


#include "TargetedAI.h"

#include "Drone_Entity.h"



class Dumb_AI : public TargetedAI {
public:
	Dumb_AI(Drone_Entity &theDrone);
	
	virtual void ApplyWithTarget(const Object &target);
	

protected:
	void SeekTarget(const Object &target, float velocityX, float velocityY);
	void SeekTarget(float speed, float targetX, float targetY, float velocityX, float velocityY);


protected:
	Drone_Entity &drone;
};

#endif