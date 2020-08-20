/* Entity.cpp
Michael Zahniser, 07/10/07

Header file for the Entity class.
*/

#include "Entity.h"

using std::fixed;



void Entity::ApplyAI()
{
	if(effect)
	{
		effect->Apply();
		if(!--effectDuration)
			effect.Reset();
	}
	else if(standardAI)
		standardAI->Apply();
}



void Entity::ApplyAI(const Object &target)
{
	lastTargetX = target.X();
	lastTargetY = target.Y();
	
	if(effect)
	{
		effect->Apply();
		if(!--effectDuration)
			effect.Reset();
	}
	else if(targetedAI)
		targetedAI->ApplyWithTarget(target);
	else
		ApplyAI();
}



void Entity::SetEffect(StandardAI::Ptr effectAI, int duration)
{
	effect = effectAI;
	effectDuration = duration;
}



	
ostream &operator<<(ostream &out, const Entity &entity)
{
	if(!entity.name.empty())
	{
		out<<fixed<<"\n\t\t"<<entity.name<<' '<<entity.X()<<' '<<entity.Y();
		
		if((entity.VelocityX() != 0.0f) | (entity.VelocityY() != 0.0) | (entity.SpriteFrame() != 0))
			out<<' '<<entity.VelocityX()<<' '<<entity.VelocityY();
		if(entity.SpriteFrame())
			out<<' '<<entity.SpriteFrame();
	}
	
	return out;
}



// Create an entity with the given (x, y) location and radius.
// If no flags are set (the default behavior), the result is a completely 
// inert object (it interacts with nothing and nothing interacts with it).
// This is not a typecast, so declare it to be an explicit constructor.
Entity::Entity(float x, float y, float collisionRadius, unsigned char typeFlags)
	: Object(x, y, collisionRadius, typeFlags), lastTargetX(-1.0f), lastTargetY(-1.0f)
{
}



// Create a new entity at the same position as the given entity, but with its
// velocity multiplied by the given scale and with the given type flags.
// Again, this must be explicit, because one of its forms has only one argument
// and therefore looks to the compiler like a typecast.
Entity::Entity(const Object &object, float velocityScale, float collisionRadius, unsigned char typeFlags)
	: Object(object, velocityScale, collisionRadius, typeFlags), lastTargetX(-1.0f), lastTargetY(-1.0f)
{
}



void Entity::SetAI(StandardAI::Ptr standard, TargetedAI::Ptr targeted)
{
	standardAI = standard;
	targetedAI = targeted;
}



void Entity::SetSprite(const string &name, int frame)
{
	spriteName = name;
	spriteFrame = frame;
}

