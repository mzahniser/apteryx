/* Object.h
Michael Zahniser, 07/08/07

Header file for the Object class, which is the base class for all objects
in the game (player, enemies, projectiles, explosions, etc.)
*/

#ifndef __MZ_OBJECT_H__
#define __MZ_OBJECT_H__


#include "TileType.h"



class Object {
public:
	virtual ~Object() {}
	
	// Get the object's (x, y) location.
	float X() const { return locationX; }
	float Y() const { return locationY; }
	float VelocityX() const { return velocityX; }
	float VelocityY() const { return velocityY; }
	float Radius() const { return radius; }
	
	// Move the object (at its current velocity) for the given time duration.
	void Move(float timeStep);
	
	// Process a possible collision between two objects.
	// This function returns true if the objects actually collide. If they collide,
	// their OnCollision() methods are also called.
	bool DoCollision(Object &object);
	// Bounce an object off the given tile at the given contact point.
	void DoBounce(float x, float y, const TileType &tileType, float restitution);
	
	// Check to see if this is a player, enemy, or upgrade.
	bool IsPlayer() const { return (type & IS_PLAYER) != 0; }
	bool IsEnemy() const { return (type & IS_ENEMY) != 0; }
	bool IsUpgrade() const { return (type & IS_UPGRADE) != 0; }
	bool IsDead() const { return (type & IS_DEAD) != 0; }
	bool IsEffect() const { return (type & 0x0F) == 0; }
	
	// Animate this object (activation, spinning, etc.).
	// This should be called once every animation frame (50 ms).
	virtual void Animate() { }
		

protected:
	// These functions are only for the use of Object subclasses.

	// These flags define what sort of object a subclass of Object is,
	// and what sorts of objects can affect it. Object::Affects(Object)
	// is used to check whether one object affects another. Note that the
	// relationship need not be reciprocal. For example, an upgrade affects
	// a player, but the player does not affect the upgrade. That is, only
	// the upgrade is informed when it collides with the player.
	static const unsigned IS_PLAYER = 0x01;
	static const unsigned IS_ENEMY = 0x02;
	static const unsigned IS_UPGRADE = 0x04;
	static const unsigned IS_DEAD = 0x08;
	static const unsigned AFFECTS_PLAYER = 0x10;
	static const unsigned AFFECTS_ENEMY = 0x20;
	static const unsigned AFFECTS_UPGRADE = 0x40;
	static const unsigned AFFECTS_DEAD = 0x80;
	
	// Create an object with the given (x, y) location and radius.
	// If no flags are set (the default behavior), the result is a completely 
	// inert object (it interacts with nothing and nothing interacts with it).
	// This is not a typecast, so declare it to be an explicit constructor.
	explicit Object(float x, float y, float collisionRadius, unsigned char typeFlags = 0);
	// Create a new object at the same position as the given object, but with its
	// velocity multiplied by the given scale and with the given type flags.
	// Again, this must be explicit, because one of its forms has only one argument
	// and therefore looks to the compiler like a typecast.
	explicit Object(const Object &object, float velocityScale = 1.0f, float collisionRadius = 0.0f, unsigned char typeFlags = 0);


protected:
	// The functions below can be overridden to define the behavior of an object.
	
	// This function is called whenever two objects collide.
	// When two objects collide, this function is called for each object if its
	// type flags mark it as affecting the other.
	// The return value is the bounce percentage; zero means no bounce.
	// If both objects return a bounce amount, the amounts are added to each other.
	virtual void OnCollide(Object &object) { }
	virtual void OnBounce(const TileType &tileType) { }
	
	// These are things that another object may try to do to this object.
	// In general, these are defined by the derived object classes.
	// Add shields. Returns false if shields can't be added (e.g. already full).
	virtual bool AddShields(float amount) { return false; }
	
	bool FirePrimary() const { return firePrimary; }
	// The secondary weapon fires once per key press, but may also be applied constantly.
	bool FireSecondary() const { if(!fireSecondary) return false; fireSecondary = false; return true; }
	bool ApplySecondary() const { return applySecondary; }
	
	void SetSpeed(float accelerationSpeed) { speed = accelerationSpeed; }
	void SetVelocity(float x, float y) { velocityX = x; velocityY = y; }
	
	float AccelerationX() const { return accelerationX; }
	float AccelerationY() const { return accelerationY; }
	
	
protected:
	// These are ways one object may affect another.
	
	// In general, these are defined by the derived object classes.
	// Add shields. Returns false if shields can't be added (e.g. already full).
	static bool AddShields(Object &object, float amount) { return object.AddShields(amount); }


private:
	// Check to see if an object is touching another object.
	bool IsTouching(const Object &object) const;
	// Modify the velocities and positions of this object and the given object
	// as a result of a collision between them.
	void Collide(Object &object, float bounce = 1.0f);
	// This is a projectile. Have it affect the given object using a simpler
	// collision algorithm that only changes the object's velocity a bit.
	void ProjectileCollide(Object &object);
	
	// An object affects another if any bit of the high half of its type flags 
	// corresponds to a bit of the low half of the other object's type flags.
	bool Affects(const Object &object) const;
	
	// "Rewind" an object's motion.
	void Rewind(float timeStep);


private:
	// All objects have an (x, y) location.
	// The location is in tiles (i.e. one map tile is 1.0 location units).
	float locationX;
	float locationY;
	
	// Velocity of the object.
	// The velocity is in tiles per time unit.
	float velocityX;
	float velocityY;
	
	// Acceleration of the object.
	float accelerationX;
	float accelerationY;
	
	// Maximum acceleration:
	float speed;
	
	// Radius of the object (in tile widths).
	// The map collision detection will fail if the radius is greater than .5;
	// implementing collision detection for larger objects would be much more
	// complicated and time-consuming.
	float radius;
	
	// Type flags. These specify what sort of object this is, and what sorts
	// of objects it interacts with.
	unsigned char type;
	
	// Primary and secondary fire activated.
	bool firePrimary;
	mutable bool fireSecondary;
	bool applySecondary;
	

private:
	// Ensure that an Object can never be instantiated unless it is also an Entity.
	virtual void IsEntity() = 0;
	
	friend class AI;
	friend class Player;
	friend class EntityList;
};

#endif
