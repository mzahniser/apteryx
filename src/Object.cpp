/* Object.cpp
Michael Zahniser, 07/08/07

Function definitions for the Object class.
*/

#include "Object.h"

#include "SoundEngine.h"

#include <cmath>

using std::sqrt;



// Move the object (at its current velocity) for the given time duration.
void Object::Move(float timeStep)
{
	float aX = accelerationX * timeStep;
	float aY = accelerationY * timeStep;
	
	locationX += (.5f * aX + velocityX) * timeStep;
	locationY += (.5f * aY + velocityY) * timeStep;

	velocityX += aX;
	velocityY += aY;	
}



// Process a possible collision between two objects.
// This function returns true if the objects actually collide. If they collide,
// their OnCollision() methods are also called.
bool Object::DoCollision(Object &object)
{
	bool thisAffectsOther = Affects(object);
	bool otherAffectsThis = object.Affects(*this);
	
	if((thisAffectsOther | otherAffectsThis) && IsTouching(object))
	{
		if((IsUpgrade() & object.IsPlayer()) | (object.IsUpgrade() & IsPlayer()))
			{}
		else if(IsEffect())
			ProjectileCollide(object);
		else if(object.IsEffect())
			object.ProjectileCollide(*this);
		else
			Collide(object);
		
		if(thisAffectsOther)
			OnCollide(object);
		
		if(otherAffectsThis)
			object.OnCollide(*this);
		
		return true;
	}
	
	return false;
}



// Bounce an object off the given point.
void Object::DoBounce(float x, float y, const TileType &tileType, float restitution)
{
	if(!IsEffect())
	{
		if(tileType.MaterialIndex() == 1)
			SoundEngine::GetInstance().Play("rock", x, y);
		else if(tileType.MaterialIndex() == 2)
			SoundEngine::GetInstance().Play("metal", x, y);
		else
			SoundEngine::GetInstance().Play("field", x, y);
	}
		
	if(x == locationX)
	{
		float time = (radius - fabs(locationY - y)) / fabs(velocityY);

		locationY -= time * velocityY * (1.0f + restitution);
		velocityY *= -restitution;

		// To allow entities to slide along walls, do not apply the restitution
		// fraction to the tangent velocity. Only do this if the normal velocity
		// is small enough.
		if(fabs(velocityY) < 1.0f)
			restitution = fabs(velocityY) * restitution + 1.0f - fabs(velocityY);
		locationX -= (1.0f - restitution) * velocityX * time;
		velocityX *= restitution;
	}
	else if(y == locationY)
	{
		float time = (radius - fabs(locationX - x)) / fabs(velocityX);

		locationX -= time * velocityX * (1.0f + restitution);
		velocityX *= -restitution;

		// To allow entities to slide along walls, do not apply the restitution
		// fraction to the tangent velocity. Only do this if the normal velocity
		// is small enough.
		if(fabs(velocityX) < 1.0f)
			restitution = fabs(velocityX) * restitution + 1.0f - fabs(velocityX);
		locationY -= (1.0f - restitution) * velocityY * time;
		velocityY *= restitution;
	}
	else {
		float time;
		{
			// Get the relative positions and velocities.
			float dx = locationX - x;
			float dy = locationY - y;
			
			// Dot products.
			float vDotV = velocityX * velocityX + velocityY * velocityY;
			float vDotD = velocityX * dx + velocityY * dy;
			float vCrossD = dx * velocityY - dy * velocityX;
			
			// This is how far we have to roll back the objects to get to
			// the moment they first come into contact.
			time = (vDotD + sqrt(vDotV * radius * radius - vCrossD * vCrossD)) / vDotV;
		}

		Rewind(time);

		{
			// Get the relative positions and velocities.
			float dx = locationX - x;
			float dy = locationY - y;
			
			// Dot products.
			float scale = 1.0f / (dx * dx + dy * dy);
			float normal = velocityX * dx + velocityY * dy;
			float tangent = velocityX * dy - velocityY * dx;
			
			velocityX = scale * (tangent * dy - restitution * normal * dx);
			velocityY = scale * (-tangent * dx - restitution * normal * dy);
		}
		
		// Move the drones forward for the same amount of time, but in the new directions.
		Rewind(-time);
	}
	
	OnBounce(tileType);
}



// Check to see if an object is touching another object.
bool Object::IsTouching(const Object &object) const
{
	float dx = locationX - object.locationX;
	float dy = locationY - object.locationY;
	
	float r = radius + object.radius;
	
	return (dx * dx + dy * dy < r * r);
}



// Modify the velocities and positions of this object and the given object
// as a result of a collision between them.
void Object::Collide(Object &object, float bounce)
{
	if(!bounce)
		return;
	
	// Get the relative velocity of the objects.
	// This will be useful at many points later on.
	float vdx = velocityX - object.velocityX;
	float vdy = velocityY - object.velocityY;
		
	// We want to find the first point when the distance between the two was
	// exactly equal to the sum of the two radii - that is, the point of contact.
	// Let d be the difference in positions, and v be the difference in velocities.
	// Solve for t in: r = magnitude(d + v*t)
	// 0 = (v dot v)t^2 + 2(v dot d)t + (d dot d - r^2)
	float time;
	{
		// Get the relative positions and velocities.
		float dx = locationX - object.locationX;
		float dy = locationY - object.locationY;
		
		float r = radius + object.radius;
		
		// Dot products.
		float vDotV = vdx * vdx + vdy * vdy;
		float vDotD = vdx * dx + vdy * dy;
		float vCrossD = dx * vdy - dy * vdx;
		
		// This is how far we have to roll back the objects to get to
		// the moment they first come into contact.
		time = (vDotD + sqrt(vDotV * r * r - vCrossD * vCrossD)) / vDotV;
	}
	
	// Back up both drones to the point of contact.
	// This uses Rewind() rather than Move() because the collision time
	// does not take acceleration into account.
	Rewind(time);
	object.Rewind(time);
	
	// Find the new distance vector d between the objects.
	float dx = locationX - object.locationX;
	float dy = locationY - object.locationY;
	
	// We will apply an acceleration to both objects that is equal to the
	// vector difference between the components of the velocities of each
	// that are in the direction of the displacement between them.
	// (That is, the difference between the normal velocity components.)
	// This object's velocity normal to the vector between the objects = 
	// d * (this object's velocity *dot* d) / (d *dot* d)
	// Same for the other object. Subtract them to get:
	float acceleration = (vdx * dx + vdy * dy) / (dx * dx + dy * dy);
	float accelerationX = acceleration * dx;
	float accelerationY = acceleration * dy;
	
	velocityX -= accelerationX;
	velocityY -= accelerationY;
	object.velocityX += accelerationX;
	object.velocityY += accelerationY;
	
	// Slow both objects down by the bounce coefficient.
	velocityX *= bounce;
	velocityY *= bounce;
	object.velocityX *= bounce;
	object.velocityY *= bounce;
	
	// Move the drones forward for the same amount of time, but in the new directions.
	Rewind(-time);
	object.Rewind(-time);
}



// This is a projectile. Have it affect the given object using a simpler
// collision algorithm that only changes the object's velocity a bit.
void Object::ProjectileCollide(Object &object)
{
	object.velocityX += .25 * velocityX;
	object.velocityY += .25 * velocityY;
}



// An object affects another if any bit of the high half of its type flags 
// corresponds to a bit of the low half of the other object's type flags.
bool Object::Affects(const Object &object) const
{
	return ((type >> 4) & object.type) != 0;
}



// "Rewind" an object's motion.
void Object::Rewind(float timeStep)
{
	locationX -= velocityX * timeStep;
	locationY -= velocityY * timeStep;
}



// Create an object. The type flags must be set when an object is created.
// If no flags are set (the default behavior), the result is a completely 
// inert object (it interacts with nothing and nothing interacts with it).
// This is not a typecast, so declare it to be an explicit constructor.
Object::Object(float x, float y, float collisionRadius, unsigned char typeFlags)
	: locationX(x), 
	locationY(y), 
	velocityX(0.0f), 
	velocityY(0.0f),
	accelerationX(0.0f), 
	accelerationY(0.0f),
	speed(0.0f),
	radius(collisionRadius), 
	type(typeFlags), 
	firePrimary(false), fireSecondary(false), applySecondary(false)
{
}



// Create a new object at the same position as the given object, but with its
// velocity multiplied by the given scale and with the given type flags.
// Again, this must be explicit, because one of its forms has only one argument
// and therefore looks to the compiler like a typecast.
Object::Object(const Object &object, float velocityScale, float collisionRadius, unsigned char typeFlags)
	: locationX(object.locationX), 
	locationY(object.locationY), 
	velocityX(velocityScale * object.velocityX), 
	velocityY(velocityScale * object.velocityY),
	accelerationX(0.0f), 
	accelerationY(0.0f),
	speed(0.0f),
	radius(collisionRadius), 
	type(typeFlags), 
	firePrimary(false), fireSecondary(false), applySecondary(false)
{
}
