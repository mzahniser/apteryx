/*
 *  Drift_AI.cpp
 *  Apteryx
 *
 *  Created by Michael Zahniser on 7/10/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Drift_AI.h"



Drift_AI::Drift_AI(Entity &entity)
	: me(entity)
{
}



void Drift_AI::Apply()
{
	AccelerateX(me, -me.VelocityX());
	AccelerateY(me, -me.VelocityY());
}
