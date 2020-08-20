/* Mindful_AI.h
Michael Zahniser, 7/11/07.

Header file for the Mindful_AI class, defining a drone AI that moves toward the
player's current position or towards the last location where it saw the player.
*/

#ifndef __MZ_MINDFUL_AI_H__
#define __MZ_MINDFUL_AI_H__


#include "Deactivate_AI.h"
#include "Dumb_AI.h"

#include "Drone_Entity.h"



class Mindful_AI : public Deactivate_AI, private Dumb_AI {
public:
	Mindful_AI(Drone_Entity &theDrone);
	
	virtual void Apply();
};

#endif