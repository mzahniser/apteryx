/* Missile.h
Michael Zahniser, 09/29/07

Header file for the Missile class, which represents a projectile.
*/

#ifndef __MZ_MISSILE_H__
#define __MZ_MISSILE_H__


#include "Projectile.h"



class Missile : public Projectile {
public:
	Missile(const Entity &source, float velocityFraction = 3.0f, int lifetime = 40);
	Missile(unsigned char typeFlags, const Entity &source, int lifetime = 40);
	
	virtual void Animate();
	
	
private:
	int life;
};

#endif