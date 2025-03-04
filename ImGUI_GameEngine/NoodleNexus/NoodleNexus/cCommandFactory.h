#pragma once

// This recieves a command in the form of a string or JSON or NV pair
//	or whatever (something type that is known)
// 
// Returns the specific command object

// Only has the iCommand interface
#include "iCommand.h"
#include <string>
#include <vector>
#include "cPhysics.h"
#include "sMesh.h"

class cCommandFactory
{
public:
	cCommandFactory();

	// If the command is NOT known, will return NULL
	iCommand* pCreateCommandObject(std::string command, 
		                           std::vector< std::string > vecDetails);

	// We will pass it the phsyics engine
	void setPhysics(cPhysics* m_pPhysicEngine);
	// Maybe also the list of meshes?
	void setVectorOfMeshes(std::vector< sMesh* >* p_vecMeshes);

	// You could have this return a list of commands,
	// or you could document this outside
private:
	cPhysics* m_pPhysicEngine;
	std::vector< sMesh* > *m_vecMeshes;
};