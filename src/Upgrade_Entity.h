/* Upgrade_Entity.h
Michael Zahniser, 09/30/07

Header file for the Upgrade_Entity class, which represents all upgrades
that can be picked up by the player.
*/

#ifndef __MZ_UPGRADE_ENTITY_H__
#define __MZ_UPGRADE_ENTITY_H__


#include "Entity.h"

#include <string>

using std::string;



class Upgrade_Entity : public Entity {
public:
	Upgrade_Entity(int variant, float x, float y);
	
	virtual void Animate();


public:
	static void Register(Entity::ConstructorMap &constructors);
	static Entity::Ptr Constructor(int variant, float x, float y);
	

protected:
	virtual void OnCollide(Object &object);


private:
	int type;
	int slowFrame;
	
	
private:
	static const int TYPES = 18;
	
	static const string NAME[TYPES];
	static const unsigned char UPGRADE[TYPES];
	static const float SHIELDS[TYPES];
	static const float ENERGY[TYPES];
};

#endif