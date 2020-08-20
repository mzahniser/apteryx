/* GlobalAI.h
Michael Zahniser, 07/10/07

Header file for the GlobalAI class, which represents an AI which the game engine can
ApplyTo() any object.
*/

#ifndef __MZ_GLOBAL_AI_H__
#define __MZ_GLOBAL_AI_H__


#include "AI.h"

#include "Entity.h"
#include "SharedPtr.h"



class GlobalAI : protected AI {
public:
	typedef SharedPtr<GlobalAI> Ptr;
	
	GlobalAI() {}
	virtual ~GlobalAI() {}

	virtual void ApplyTo(Entity &target) const;


private:
	// Disallow copying.
	GlobalAI(const GlobalAI &);
	GlobalAI &operator=(const GlobalAI &);
};

#endif