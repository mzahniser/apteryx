/* Mine_Projectile.h
Michael Zahniser, 10/02/07

Header file for the Mine_Projectile class, which represents a mine fired by a player.
*/

#ifndef __MZ_MINE_PROJECTILE_H__
#define __MZ_MINE_PROJECTILE_H__


#include "Projectile.h"



class Mine_Projectile : public Projectile {
public:
	Mine_Projectile(const Entity &source);
	

protected:
	virtual void OnCollide(Object &object);
	virtual void Die();
};

#endif