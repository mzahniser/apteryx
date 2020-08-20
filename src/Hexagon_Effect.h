/* Hexagon_Effect.h
Michael Zahniser, 10/03/07

Header file for the Hexagon_Effect class, which represents the "emergency hexagon"
temporary shield.
*/

#ifndef __MZ_HEXAGON_EFFECT_H__
#define __MZ_HEXAGON_EFFECT_H__


#include "Entity.h"



class Hexagon_Effect : public Entity {
public:
	Hexagon_Effect(const Entity &source, int lifetime);

	virtual void Animate();


protected:
	virtual void OnCollide(Object &object);
	
	
private:
	int life;
	
	float velocityX;
	float velocityY;
};

#endif