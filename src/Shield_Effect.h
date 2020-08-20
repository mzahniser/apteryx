/* Shield_Effect.h
Michael Zahniser, 09/29/07

Header file for the Shield_Effect class, which represents the shield splash displayed
when anything impacts the player's shields.
*/

#ifndef __MZ_SHIELD_EFFECT_H__
#define __MZ_SHIELD_EFFECT_H__


#include "Entity.h"



class Shield_Effect : public Entity {
public:
	// The sprite will be chosen based on the direction of impact.
	Shield_Effect(const Object &object, float dx, float dy);

	// Get the index of the sprite to use for this object, given the current time.
	virtual void Animate();
};

#endif