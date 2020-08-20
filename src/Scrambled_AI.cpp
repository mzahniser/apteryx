/* Scrambled_AI.cpp
Michael Zahniser, 10/22/07

Function definitions for the Scrambled_AI class.
*/

#include "Scrambled_AI.h"



Scrambled_AI::Scrambled_AI(Entity &entity)
	: me(entity)
{
}



void Scrambled_AI::Apply()
{
	AccelerateX(me, me.VelocityX());
	AccelerateY(me, me.VelocityY());
	FirePrimary(me);	
}
