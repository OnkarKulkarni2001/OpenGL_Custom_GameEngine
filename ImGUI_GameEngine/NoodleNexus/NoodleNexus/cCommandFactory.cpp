#include "cCommandFactory.h"

// Abstract factory: includes specific classes here
// NEVER in the header
#include "cMoveRelativeTime.h"
#include "cFollowObject_A.h"

#include <iostream>



cCommandFactory::cCommandFactory()
{
	this->m_pPhysicEngine = NULL;
	this->m_vecMeshes = NULL;
}

void cCommandFactory::setPhysics(cPhysics* m_pPhysicEngine)
{
	this->m_pPhysicEngine = m_pPhysicEngine;
	return;
}

// Maybe also the list of meshes?
void cCommandFactory::setVectorOfMeshes(std::vector< sMesh* >* p_vecMeshes)
{
	this->m_vecMeshes = p_vecMeshes;
	return;
}


// If the command is NOT known, will return NULL
iCommand* cCommandFactory::pCreateCommandObject(
	std::string command,
	std::vector< std::string > vecDetails)
{
	// Determine which commands I want
	if (command == "Move Relative ConstVelocity+Time")
	{
		cMoveRelativeTime* pMoveCommand = new cMoveRelativeTime();
		// [0] = friendly name of the object to control
		// [1]-[3] = x,y,z destination of the obejct
		// [4] = time to move 
		// 
		// TODO: Maybe add some error handling if the command is wrong
		// (for now, we are just going to "go for it"!

		cPhysics::sPhysInfo* pMeshToControl = 
			this->m_pPhysicEngine->pFindAssociateMeshByFriendlyName(vecDetails[0]);		// "New_Viper_Player"
		//
		glm::vec3 destinationXYZ = 
			glm::vec3( atof(vecDetails[1].c_str()),
		               atof(vecDetails[2].c_str()),
		               atof(vecDetails[3].c_str()) );
		//		
		double timeToMove = atof(vecDetails[4].c_str());

		// Finally call the Init
		pMoveCommand->Init(pMeshToControl, destinationXYZ, timeToMove);

		return pMoveCommand;
	}
	if (command == "Follow Object, offset & max velocity")
	{
		iCommand* pFollow = new cFollowObject_A();


		return pFollow;
	}



	// Don't know that command
	return NULL;
}
