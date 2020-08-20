/* Dead_Entity.cpp
Michael Zahniser, 7/12/07.

Function definitions for the Dead_Entity class.
*/

#include "Dead_Entity.h"

#include "Drift_AI.h"
#include "SoundEngine.h"

#include <cstdlib>

using std::rand;



Dead_Entity::Dead_Entity(int variant, float x, float y)
	: Entity(x, y, .3, IS_DEAD | AFFECTS_ENEMY | AFFECTS_PLAYER | AFFECTS_DEAD),
	droneType(variant)
{
	Initialize();
}



Dead_Entity::Dead_Entity(const Entity &entity, int variant)
	: Entity(entity, 1.0f, .3f, IS_DEAD | AFFECTS_ENEMY | AFFECTS_PLAYER | AFFECTS_DEAD),
	droneType(variant)
{
	Initialize();
}



void Dead_Entity::Register(Entity::ConstructorMap &constructors)
{
	for(int i = 0; i < TYPES; ++i)
		constructors[NAME[i]] = VariantConstructor(&Constructor, i);
}



Entity::Ptr Dead_Entity::Constructor(int variant, float x, float y)
{
	return Entity::Ptr(new Dead_Entity(variant, x, y));
}



void Dead_Entity::OnCollide(Object &object)
{
	SoundEngine::GetInstance().Play("metal", X(), Y());
}



// Initialization shared between the two constructors.
void Dead_Entity::Initialize()
{
	SetAI(StandardAI::Ptr(new Drift_AI(*this)));
		
	SetName(NAME[droneType]);
	SetSprite(NAME[droneType], rand() % 3);
	
	// This controls how fast a dead entity slows down to a stop.
	SetSpeed(3.0f);	
}



const string Dead_Entity::NAME[TYPES] = {
	"deadRed",
	"deadYellow",
	"deadGreen",
	"deadBlue",
	"deadGrey",
	"deadPlayer"
};
