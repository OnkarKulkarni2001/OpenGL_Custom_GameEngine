#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
//#include "iMessaging.h"
class iMessaging;

class iTank
{
public:
	// If this is a virtual interface, 
	// make sure that you add a virtual destructor
	virtual ~iTank() {}


	// Pure vitual method 
	// "= 0" mean there is no method
//	virtual void Attack(/*another tank*/) = 0;
	virtual void Attack(iTank *pEnemyTank) = 0;
//	virtual void Attack(glm::vec3 location) = 0;

	virtual glm::vec3 getLocation(void) = 0;
	virtual void setLocation(glm::vec3 newLocation_) = 0;
	virtual void UpdateTick(double deltaTime) = 0;

	virtual unsigned int getID(void) = 0;

	virtual void setMediator(iMessaging* pTheMediator) = 0;

};
