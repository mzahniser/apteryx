/* Explosion_Effect.h
Michael Zahniser, 09/29/07

Header file for the Explosion_Effect class, which represents an animated explosion.
*/

#ifndef __MZ_EXPLOSION_EFFECT_H__
#define __MZ_EXPLOSION_EFFECT_H__


#include "Entity.h"



class Explosion_Effect : public Entity {
public:
	Explosion_Effect(const string &spriteName, const Entity &entity, float relativeVelocity = 1.0f);

	// Get the index of the sprite to use for this object, given the current time.
	virtual void Animate();
};

#endif