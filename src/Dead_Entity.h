/* Dead_Entity.h
Michael Zahniser, 7/12/07.

Header file for the Dead_Entity class, which represents a drone that has been destroyed.
Dead drones will drift to a stop, and bounce off the player or other drones. They also destroy
any live drones that collide with them.
*/

#ifndef __MZ_DEAD_ENTITY_H__
#define __MZ_DEAD_ENTITY_H__


#include "Entity.h"



class Dead_Entity : public Entity {
public:
	Dead_Entity(int variant, float x, float y);
	Dead_Entity(const Entity &entity, int variant);
	
	
public:
	static void Register(Entity::ConstructorMap &constructors);
	static Entity::Ptr Constructor(int variant, float x, float y);
	

protected:
	virtual void OnCollide(Object &object);


private:
	// Initialization shared between the two constructors.
	void Initialize();
	
	
private:
	int droneType;
	
	
private:
	static const int TYPES = 6;
	static const string NAME[TYPES];
};

#endif