/* Laser_Projectile.h
Michael Zahniser, 09/29/07

Header file for the Laser_Projectile class, which represents a laser fired by a player.
*/

#ifndef __MZ_LASER_PROJECTILE_H__
#define __MZ_LASER_PROJECTILE_H__


#include "Projectile.h"



class Laser_Projectile : public Projectile {
public:
	Laser_Projectile(const Entity &source);
	

protected:
	virtual void OnCollide(Object &object);
};

#endif