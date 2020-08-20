/* EMPBurst_AI.cpp
Michael Zahniser, 7/11/07.

Function definitions for the EMPBurst_AI class.
*/

#include "EMPBurst_AI.h"

#include "Entity.h"
#include "Deactivate_AI.h"
#include "LevelChanges.h"
#include "Explosion_Effect.h"



EMPBurst_AI::EMPBurst_AI(int effectDuration)
	: duration(effectDuration)
{
}


	
void EMPBurst_AI::ApplyTo(Entity &target) const
{
	if(target.IsEnemy())
	{
		Drone_Entity &entity = reinterpret_cast<Drone_Entity &>(target);

		entity.SetEffect(StandardAI::Ptr(new Deactivate_AI(entity)), duration);
		
		LevelChanges::GetInstance().Add(new Explosion_Effect("empStrike", entity));
	}
}
