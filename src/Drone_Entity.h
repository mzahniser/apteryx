/* Drone_Entity.h
Michael Zahniser, 7/10/07.

Header file for the Drone_Entity class, which represents a drone that follows the player
(if visible) and tries to crash into the player, and may also fire lasers at the player.
*/

#ifndef __MZ_DRONE_ENTITY_H__
#define __MZ_DRONE_ENTITY_H__


#include "Entity.h"

#include <string>

using std::string;



class Drone_Entity : public Entity {
public:
	Drone_Entity(int variant, float x, float y);
	
	virtual void Animate();
	
	void SetTargetIsVisible(bool isVisible) { targetIsVisible = isVisible; }
	bool IsActive() const { return isActive; }
	
	
public:
	static void Register(Entity::ConstructorMap &constructors);
	static Entity::Ptr Constructor(int variant, float x, float y);
	

protected:
	virtual void OnCollide(Object &object);
	virtual void OnBounce(const TileType &tileType);

	// Add shields. Returns false if shields can't be added (e.g. already full).
	virtual bool AddShields(float amount);
	
	
protected:
	int droneType;
	// Allow ControlledDrone constructors access to a specific drone's type:
	static int Type(const Drone_Entity &drone) { return drone.droneType; }
	
	
private:
	void Die();
	
	
private:
	unsigned char reloadTime;
	unsigned char reload;
	
	float shields;
	
	bool targetIsVisible;
	bool isActive;
		
	
protected:
	static const int TYPES = 6;
	static const string NAME[TYPES];
};

#endif