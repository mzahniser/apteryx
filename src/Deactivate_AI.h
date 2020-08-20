/* Deactivate_AI.h
Michael Zahniser, 7/11/07.

Header file for the Deactivate_AI class, which defines the behavior of a drone when it
is deactivating itself or when it is not active.
*/

#ifndef __MZ_DEACTIVATE_AI_H__
#define __MZ_DEACTIVATE_AI_H__


#include "Drift_AI.h"

#include "Drone_Entity.h"



class Deactivate_AI : public Drift_AI {
public:
	Deactivate_AI(Drone_Entity &theDrone);
	
	virtual void Apply();
	
	
protected:
	Drone_Entity &drone;
};

#endif