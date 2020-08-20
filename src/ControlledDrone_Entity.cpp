/*
 *  ControlledDrone_Entity.cpp
 *  Apteryx
 *
 *  Created by Michael Zahniser on 10/3/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "ControlledDrone_Entity.h"

#include "GameData.h"

#include "Deactivate_AI.h"



ControlledDrone_Entity::ControlledDrone_Entity(const Drone_Entity &source)
	: Drone_Entity(Drone_Entity::Type(source), source.X(), source.Y())
{
	Initialize();
	
	SetVelocity(source.VelocityX(), source.VelocityY());
}



ControlledDrone_Entity::ControlledDrone_Entity(int variant, float x, float y)
	: Drone_Entity(variant, x, y)
{
	Initialize();
}



void ControlledDrone_Entity::Animate()
{
	SetTargetIsVisible(GameData::GetInstance().ControlledEntity() == this);
	
	Drone_Entity::Animate();
}



void ControlledDrone_Entity::Register(Entity::ConstructorMap &constructors)
{
	for(int i = 0; i < Drone_Entity::TYPES; ++i)
		constructors[ControlledName(i)] = Entity::VariantConstructor(&Constructor, i);
}



Entity::Ptr ControlledDrone_Entity::Constructor(int variant, float x, float y)
{
	return Entity::Ptr(new ControlledDrone_Entity(variant, x, y));
}



// Initialization common to all constructors.
void ControlledDrone_Entity::Initialize()
{
	SetAI(StandardAI::Ptr(new Deactivate_AI(*this)));
	
	// Change my name from "droneColor" to "controlledColor".
	SetName(ControlledName(droneType));
}



	
string ControlledDrone_Entity::ControlledName(int variant)
{
	return "controlled" + Drone_Entity::NAME[variant].substr(5);
}