/* Controlled_AI.h
Michael Zahniser, 09/26/07

Header file for the Controlled_AI class, which moves an entity under player control.
*/

#ifndef __MZ_CONTROLLED_AI_H__
#define __MZ_CONTROLLED_AI_H__


#include "GlobalAI.h"



class Controlled_AI : public GlobalAI {
public:
	Controlled_AI();
	
	virtual void ApplyTo(Entity &target) const;
	
	enum Direction {
		LEFT,
		UP,
		RIGHT,
		DOWN,
		PRIMARY,
		SECONDARY
	};
	
	void KeyDown(Direction direction);
	void KeyUp(Direction direction);
	
	void CheatSetVelocity(float vx, float vy);
	

private:
	bool left;
	bool up;
	bool right;
	bool down;
	
	bool firePrimary;
	bool fireSecondary;
	
	bool cheat;
	float velocityX;
	float velocityY;
};

#endif