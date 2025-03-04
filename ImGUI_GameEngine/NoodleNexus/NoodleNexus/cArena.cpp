#include "cArena.h"

// From the iMessaging interface
// // Sent to the recieving object
bool cArena::Receive_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code
	if (theMesssage.theCommand == "FindFarthestTank")
	{
		// “FindNearestTank”: our location
		// 1st thing on the vector is the location of the tank
		glm::vec3 callingTankXYZ = theMesssage.vecDetails[0];

		// Find the nearest tank
		glm::vec3 theTarget = callingTankXYZ;
		float maxDistanceSoFar = 0.0f;
		for (iTank* pCurTank : this->m_vec_pTanks)
		{
			float tankDistance = glm::distance(pCurTank->getLocation(), callingTankXYZ);
			if (tankDistance > maxDistanceSoFar)
			{
				// New max distance
				maxDistanceSoFar = tankDistance;
				theTarget = pCurTank->getLocation();
			}
		}
		// Has found a target. Check if it's "too close" 
		// (i.e. will damage firing tank)
		const float MIN_SAFE_FIRING_DISTANCE = 10.0f;
		if (maxDistanceSoFar < MIN_SAFE_FIRING_DISTANCE)
		{
			// We DON'T have a target

		}
		else
		{
			// Return the target location
		}


		// Mediator sends : “ClosestTank” + location to shoot

	}// "FindFarthestTank" )


	return true;
}



// From the iMessaging interface
// // Sent to the mediator to pass along
bool cArena::Send_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}

void cArena::AddTank(iTank* pNewTank)
{
	this->m_vec_pTanks.push_back(pNewTank);
	return;
}



void cArena::AddEntity(iMessaging* pTheThing)
{
	this->m_vec_pThings.push_back(pTheThing);
	return;
}

