/* Virus_Projectile.h
Michael Zahniser, 10/03/07

Header file for the Virus_Projectile class, which represents a mine fired by a player.
*/

#ifndef __MZ_VIRUS_PROJECTILE_H__
#define __MZ_VIRUS_PROJECTILE_H__


#include "Missile.h"



class Virus_Projectile : public Missile {
public:
	Virus_Projectile(const Entity &source);
	

protected:
	virtual void OnCollide(Object &object);
	virtual void Die();
};

#endif