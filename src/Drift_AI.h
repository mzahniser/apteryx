/* Drift_AI.h
Michael Zahniser, 07/10/07

Header file for the Drift_AI, an AI that causes a moving object to slow down
to a standstill.
*/

#ifndef __MZ_DRIFT_AI_H__
#define __MZ_DRIFT_AI_H__


#include "StandardAI.h"

#include "Entity.h"



class Drift_AI : public StandardAI {
public:
	Drift_AI(Entity &entity);
	
	virtual void Apply();
	
	
private:
	Entity &me;
};

#endif