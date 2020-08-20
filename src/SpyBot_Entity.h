/* SpyBot_Entity.h
Michael Zahniser, 10/22/07

Header file for the SpyBot_Entity class, which represents the player's ship.
*/

#ifndef __MZ_SPY_BOT_ENTITY_H__
#define __MZ_SPY_BOT_ENTITY_H__


#include "Entity.h"



class SpyBot_Entity : public Entity {
public:
	SpyBot_Entity(float x, float y);

	virtual void Animate();

	virtual bool IsControlled() const { return true; }
	
	
public:
	static void Register(Entity::ConstructorMap &constructors);
	static Entity::Ptr Constructor(int variant, float x, float y);
};

#endif