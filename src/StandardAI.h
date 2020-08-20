/* StandardAI.h
Michael Zahniser, 07/10/07

Header file for the StandardAI class, which represents an AI that is initialized with a reference
to an object and applies the AI to that object whenever Apply() is called.
*/

#ifndef __MZ_STANDARD_AI_H__
#define __MZ_STANDARD_AI_H__


#include "AI.h"

#include "SharedPtr.h"



class StandardAI : protected AI {
public:
	typedef SharedPtr<StandardAI> Ptr;
	
	StandardAI() {}
	virtual ~StandardAI() {}
	
	virtual void Apply() {}


private:
	// Disallow copying.
	StandardAI(const StandardAI &);
	StandardAI &operator=(const StandardAI &);
};

#endif