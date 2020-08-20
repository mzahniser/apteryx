/* Scrambled_AI.h
Michael Zahniser, 10/22/07

Header file for the Scrambled_AI, an AI that defines an enemy's behavior under
the Scrambler effect.
*/

#ifndef __MZ_SCRAMBLED_AI_H__
#define __MZ_SCRAMBLED_AI_H__


#include "StandardAI.h"

#include "Entity.h"



class Scrambled_AI : public StandardAI {
public:
	Scrambled_AI(Entity &entity);
	
	virtual void Apply();
	
	
private:
	Entity &me;
};

#endif