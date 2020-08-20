/* Hovercraft_Entity.h
Michael Zahniser, 09/26/07

Header file for the Hovercraft_Entity class, which represents the player's ship.
*/

#ifndef __MZ_HOVERCRAFT_ENTITY_H__
#define __MZ_HOVERCRAFT_ENTITY_H__


#include "Entity.h"

#include <string>

using std::string;



class Hovercraft_Entity : public Entity {
public:
	Hovercraft_Entity(float x, float y);
	
	virtual void Animate();

	virtual bool IsControlled() const { return true; }
	
	
public:
	static void Register(Entity::ConstructorMap &constructors);
	static Entity::Ptr Constructor(int variant, float x, float y);
	

protected:
	virtual void OnCollide(Object &object);

	// Add shields. Returns false if shields can't be added (e.g. already full).
	virtual bool AddShields(float amount);
	
	
private:
	int slowFrame;
};

#endif