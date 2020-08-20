/* TargetedAI.h
Michael Zahniser, 07/10/07

Header file for the TargetedAI class, which represents an AI that is initialized with a reference
to an object and applies the AI to that object whenever ApplyWithTarget() is called.
*/

#ifndef __MZ_TARGETED_AI_H__
#define __MZ_TARGETED_AI_H__


#include "AI.h"

#include "Object.h"
#include "SharedPtr.h"



class TargetedAI : protected AI {
public:
	typedef SharedPtr<TargetedAI> Ptr;
	
	TargetedAI() {}
	virtual ~TargetedAI() {}
	
	virtual void ApplyWithTarget(const Object &target) {}


private:
	// Disallow copying.
	TargetedAI(const TargetedAI &);
	TargetedAI &operator=(const TargetedAI &);
};

#endif