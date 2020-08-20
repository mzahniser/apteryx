/* ControlledDrone_Entity.h
Michael Zahniser, 10/03/07

Header file for the ControlledDrone class, which represents a drone under player control.
*/

#ifndef __MZ_CONTROLLED_DRONE_H__
#define __MZ_CONTROLLED_DRONE_H__


#include "Drone_Entity.h"

#include <string>

using std::string;



class ControlledDrone_Entity : public Drone_Entity {
public:
	ControlledDrone_Entity(const Drone_Entity &source);
	ControlledDrone_Entity(int variant, float x, float y);
	
	virtual void Animate();

	virtual bool IsControlled() const { return true; }


public:
	static void Register(Entity::ConstructorMap &constructors);
	static Entity::Ptr Constructor(int variant, float x, float y);
	
	
private:
	// Initialization common to all constructors.
	void Initialize();
	
	static string ControlledName(int variant);
};

#endif