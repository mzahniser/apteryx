/* AI.h
Michael Zahniser, 07/10/07

Header file for the AI base class, which defines the actions that AIs are allowed
to perform on objects.
*/

#ifndef __MZ_AI_H__
#define __MZ_AI_H__


#include "Object.h"



class AI {
protected:
	static void AccelerateX(Object &object, int direction);
	static void AccelerateY(Object &object, int direction);

	static void AccelerateX(Object &object, float direction);
	static void AccelerateY(Object &object, float direction);
	
	float Speed(const Object &object) { return object.speed; }
	
	static void FirePrimary(Object &object, bool shouldFire = true);
	static void ApplySecondary(Object &object, bool isApplied = true);
	
	// "Cheater" option to set velocity directly.
	static void CheatVelocityX(Object &object, float velocity);
	static void CheatVelocityY(Object &object, float velocity);
};

#endif