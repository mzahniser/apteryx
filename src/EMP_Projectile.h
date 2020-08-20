/* EMP_Projectile.h
Michael Zahniser, 10/02/07

Header file for the EMP_Projectile class, which represents an EMP shot fired by a player.

An EMP shot with negative or zero duration (i.e. permanent effect) is a virus.
*/

#ifndef __MZ_EMP_PROJECTILE_H__
#define __MZ_EMP_PROJECTILE_H__


#include "Projectile.h"



class EMP_Projectile : public Projectile {
public:
	EMP_Projectile(const Entity &source, int effectDuration);
	

protected:
	virtual void OnCollide(Object &object);
	virtual void Die();
	
	
private:
	int duration;
};

#endif