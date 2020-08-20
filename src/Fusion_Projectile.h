/* Fusion_Projectile.h
Michael Zahniser, 10/02/07

Header file for the Fusion_Projectile class, which represents a mine fired by a player.
*/

#ifndef __MZ_FUSION_PROJECTILE_H__
#define __MZ_FUSION_PROJECTILE_H__


#include "Projectile.h"



class Fusion_Projectile : public Projectile {
public:
	Fusion_Projectile(const Entity &source);
	Fusion_Projectile(const Entity &splitFrom, int typeFlags);

	virtual void Animate();


protected:
	virtual void OnCollide(Object &object);
	virtual void Die();
};

#endif