#include "cMoveRelativeTime.h"

void cMoveRelativeTime::OnStart(void)
{
	// TODO: 
	this->m_elapsedTime = 0.0;
	// Calcuate the velocity required to get this object to that location
	glm::vec3 totalDistance = this->m_endXYZ - this->m_pObject->position;
	// What velocity will get us there?
	this->m_pObject->velocity = totalDistance / (float)this->m_timeToMove;

	return;
}

void cMoveRelativeTime::Update(double deltaTime)
{
	// Let the physics thing move the object
	// Keep track of the elapsed time
	this->m_elapsedTime += deltaTime;
	return;
}
bool cMoveRelativeTime::isFinished(void)
{
	// Has the amount of time passed
	if (this->m_elapsedTime >= this->m_timeToMove)
	{
		// We've arrived
		return true;
	}

	// Keep going...
	return false;
}

void cMoveRelativeTime::OnFinished(void)
{
	// We've arrived
	this->m_pObject->velocity = glm::vec3(0.0f);
	this->m_pObject->acceleration = glm::vec3(0.0f);
	return;
}


void cMoveRelativeTime::Init(cPhysics::sPhysInfo* pObject, glm::vec3 endXYZ, double timeToMove)
{
	this->m_timeToMove = timeToMove;
	this->m_pObject = pObject;
	this->m_endXYZ = endXYZ;
	this->m_elapsedTime = 0.0;

	return;
}
