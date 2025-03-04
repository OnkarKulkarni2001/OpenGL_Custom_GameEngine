#pragma once

#include "iCommand.h"

// Follow an object at a certain distance. 
// Pass a mesh pointer and a min distance and 
//	a max velocity and an offset
class cFollowObject_A : public iCommand
{
public:
	//cFollowObject_A

	// From the iCommand interface
	virtual void OnStart(void);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);
	virtual void OnFinished(void);
};
