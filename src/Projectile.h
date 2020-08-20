/* Projectile.h
Michael Zahniser, 09/29/07

Header file for the Projectile class, which represents a projectile.
*/

#ifndef __MZ_PROJECTILE_H__
#define __MZ_PROJECTILE_H__


#include "Entity.h"



class Projectile : public Entity {
public:
	Projectile(const Entity &source, float velocityFraction = 3.0f, int lifetime = 40);
	Projectile(unsigned char typeFlags, const Entity &source, int lifetime = 40);
	
	virtual void Animate();
	
	
protected:
	virtual void OnBounce(const TileType &tileType);
	virtual void Die() {}
	
	int LifeRemaining() const { return life; }
	
	void SetFrames(int frameCount) { frames = frameCount; }
	
	
private:
	int life;
	int frames;
};

#endif