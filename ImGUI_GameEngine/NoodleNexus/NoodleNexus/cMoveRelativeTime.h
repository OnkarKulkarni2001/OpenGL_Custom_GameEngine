#pragma once

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "cPhysics.h"

// Move from the current location to a new location
// ... over a certain amount of time
class cMoveRelativeTime : public iCommand
{
public:
	// Call this to set the values
	// This specific to this particular command
	void Init(cPhysics::sPhysInfo* pObject, glm::vec3 endXYZ, double timeToMove);

	// From iCommand interface
	virtual void OnStart(void);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);
	virtual void OnFinished(void);

private:
	glm::vec3 m_endXYZ;
	double m_timeToMove;
	cPhysics::sPhysInfo* m_pObject;
	// This is calculated
	double m_elapsedTime;
};
