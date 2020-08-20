/* Entity.h
Michael Zahniser, 07/10/07

Header file for the Entity class. An Entity is an Object with volition; that is, an Object that
has an AI associated with it.
*/

#ifndef __MZ_ENTITY_H__
#define __MZ_ENTITY_H__


#include "Object.h"

#include "StandardAI.h"
#include "TargetedAI.h"
#include "SharedPtr.h"

#include <string>
#include <map>
#include <utility>
#include <ostream>

using std::string;
using std::map;
using std::pair;
using std::ostream;



class Entity : public Object {
public:
	virtual ~Entity() {}
	
	const string &Name() const { return name; }
	const string &SpriteName() const { return spriteName; }
	int SpriteFrame() const { return spriteFrame; }
	void SetSpriteFrame(int frame) { spriteFrame = frame; }
	
	// Functions to apply the object's AIs.
	void ApplyAI();
	void ApplyAI(const Object &target);
	
	// Apply a temporary effect to a drone. For a permanent effect (i.e. virus)
	// give a duration of <= 0.
	void SetEffect(StandardAI::Ptr effectAI, int duration);
	
	virtual bool IsControlled() const { return false; }
	
	friend ostream &operator<<(ostream &out, const Entity &entity);
	
	float LastTargetX() const { return lastTargetX; }
	float LastTargetY() const { return lastTargetY; }
	
	
public:
	typedef SharedPtr<Entity> Ptr;
	typedef Ptr (*Constructor)(int variant, float x, float y);
	typedef pair<Constructor, int> VariantConstructor;
	typedef map<string, VariantConstructor> ConstructorMap;
	
	
protected:
	// Functions allowing derived classes to create objects.
	
	// Create an entity with the given (x, y) location and radius.
	// If no flags are set (the default behavior), the result is a completely 
	// inert object (it interacts with nothing and nothing interacts with it).
	// This is not a typecast, so declare it to be an explicit constructor.
	explicit Entity(float x, float y, float collisionRadius, unsigned char typeFlags = 0);
	// Create a new entity at the same position as the given entity, but with its
	// velocity multiplied by the given scale and with the given type flags.
	// If a default argument is given for the velocity scale, this can be mistaken by
	// the compiler for the copy constructor, so velocityScale must be specified.
	// The other option is to trust all callers of this constructor to downcast to Object &
	// before passing the object as an argument, but this is safer.
	explicit Entity(const Object &object, float velocityScale, float collisionRadius = 0.0f, unsigned char typeFlags = 0);

	// Set the AIs used by an entity.
	void SetAI(StandardAI::Ptr standard, TargetedAI::Ptr targeted = TargetedAI::Ptr());

	void SetName(const string &entityName) { name = entityName; }
	void SetSprite(const string &name, int frame = 0);

	
private:
	StandardAI::Ptr standardAI;
	TargetedAI::Ptr targetedAI;
	
	StandardAI::Ptr effect;
	int effectDuration;
	
	string name;
	string spriteName;
	int spriteFrame;
	
	float lastTargetX;
	float lastTargetY;
	
	
private:
	// Ensure that an Object can never be instantiated unless it is also an Entity.
	virtual void IsEntity() {};
	
	// Do not allow copying.
	explicit Entity(const Entity &);
	Entity &operator=(const Entity &);
};

#endif