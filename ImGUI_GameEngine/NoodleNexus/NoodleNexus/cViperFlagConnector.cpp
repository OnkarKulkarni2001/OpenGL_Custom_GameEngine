#include "cViperFlagConnector.h"
#include <algorithm>

void cViperFlagConnector::setViperModel(sMesh* pViperPlayer)
{
	this->m_pViperPlayer = pViperPlayer;
	return;
}

void cViperFlagConnector::setFlagSoftBody(cSoftBodyVerlet* pFlagModel)
{
	this->m_pFlag = pFlagModel;
	return;
}


// Predicate function: less than
// is A less than B where A & B are particles
// std::vector< cSoftBodyVerlet::sParticle* > m_vecParticles;
bool PF_ParticleLessThan(
	cSoftBodyVerlet::sParticle* A,
	cSoftBodyVerlet::sParticle* B)
{
	if (A->position.x < B->position.x)
	{
		return true;
	}
	return false;
}


bool cViperFlagConnector::ConnectViperToFlag(void)
{
	// Flag is along the x axis, from -15 to 15
	// Viper is along the y axis, from -15 to 15
	
	// 1. Find all the vertices along the flags X axis
	//    Add all the vertices that are "close enough" to -15 in the z

	for ( cSoftBodyVerlet::sParticle* pCurParticle : this->m_pFlag->vec_pParticles )
	{
		// Is this "close enough" to -15?
		// (how about if it's Less Than -14.9?)
		if (pCurParticle->position.z < -14.9f)
		{
			// Close enough. Add to the vector
			this->m_vecParticles.push_back(pCurParticle);
		}
	}
	
	// Sort them from lowest to highest along the x axis
	std::sort(this->m_vecParticles.begin(), this->m_vecParticles.end(), PF_ParticleLessThan);

	// 2. Make all of these particles "fixed" (i.e. DON'T update with Verlet)
	for (cSoftBodyVerlet::sParticle* pParticle : this->m_vecParticles)
	{
		pParticle->bIsFixed_DontUpdate = true;
	}

	// 3. Connect these particles to the bottom of the viper
	this->UpdateFlagLocation();

	return true;
}




// Called every frame to update the location of that flag edge
void cViperFlagConnector::UpdateFlagLocation(void)
{
	// The particles are already sorted from one edge to the other.
	// We are connecting them to the bottom of the viper in that order. 

	// Viper is oriented along the z and is about -15 to 15 compared to the origin
	
	// TODO: We might want to take into accoun the orientation of the viper...

	// Here, I'm ignoring that because we can't rotate the viper in this example
	// (the "nose" of the viper is at -15.0f units from the vipers location
	glm::vec3 minFlagEdge = this->m_pViperPlayer->positionXYZ - glm::vec3(0.0f, 0.0f, -15.0f);
	// (...and the "tail" is +15.0f units from the vipers current location
	glm::vec3 maxFlagEdge = this->m_pViperPlayer->positionXYZ - glm::vec3(0.0f, 0.0f, +15.0f);

	unsigned int numberOfParticles = (unsigned int)this->m_vecParticles.size();
	// The spacing of the particles is max - min / numberOfParticles

	glm::vec3 particleSpacing = (maxFlagEdge - minFlagEdge) / (float)numberOfParticles;

	// move the particles in the soft body to the new location, just below the viper
	// Remember: the particles are IN ORDER from the model (along the endge of the flag)
	glm::vec3 curParticleLocation = minFlagEdge;
	// And a little below the viper (in the y axis)
	curParticleLocation.y -= 5.0f;			// 5 units below the viper

	for (cSoftBodyVerlet::sParticle* pParticle : this->m_vecParticles)
	{
		pParticle->position = curParticleLocation;
		// 
		curParticleLocation += particleSpacing;
	}

	return;
}

// Set all the particles to be updated by Verlet 
// (i.e. they AREN'T being ignored)
void cViperFlagConnector::ReleaseFlag(void)
{
	this->UpdateFlagLocation();

	// 1. set all the particles to be updated
	for (cSoftBodyVerlet::sParticle* pParticle : this->m_vecParticles)
	{
		pParticle->bIsFixed_DontUpdate = false;
	}

	// 2. remove the particles from the vector we are 
	//		using to connect to viper
	this->m_vecParticles.clear();

	return;
}
