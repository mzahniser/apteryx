/* SpyBot_Entity.cpp
Michael Zahniser, 10/22/07

Function definitions for the SpyBot_Entity class.
*/

#include "SpyBot_Entity.h"

#include "Drift_AI.h"



SpyBot_Entity::SpyBot_Entity(float x, float y)
	: Entity(x, y, .35f)
{
	SetAI(StandardAI::Ptr(new Drift_AI(*this)));
	
	SetSpeed(3.0f);
	
	SetName("spyBot");
	SetSprite("spyBot", 4);
}



void SpyBot_Entity::Animate()
{
	int directionX = (AccelerationX() > 0.0f) - (AccelerationX() < 0.0f);
	int directionY = (AccelerationY() > 0.0f) - (AccelerationY() < 0.0f);
	
	SetSpriteFrame(4 - directionY * 3 - directionX);	
}



void SpyBot_Entity::Register(Entity::ConstructorMap &constructors)
{
	constructors["spyBot"] = Entity::VariantConstructor(&Constructor, 0);
}



Entity::Ptr SpyBot_Entity::Constructor(int variant, float x, float y)
{
	return Entity::Ptr(new SpyBot_Entity(x, y));
}
