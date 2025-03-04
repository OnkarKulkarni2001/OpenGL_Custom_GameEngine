#pragma once

#include "sMesh.h"
#include "cSoftBodyVerlet.h"
#include <vector>

//
// Make one edge of the flag be stuck to the bottom of the viper
// (so we can move the viper and see what happens)
// Here, you make the particles (vertices, etc.) in the soft body
// "fixed" and you update them every frame 
// 

class cViperFlagConnector
{
public:
	// Called once to set the edges of the flag to the viper
	bool ConnectViperToFlag(void);
	// Called every frame to update the location of that flag edge
	void UpdateFlagLocation(void);

	void setViperModel(sMesh* pViperPlayer);
	void setFlagSoftBody(cSoftBodyVerlet* pFlagModel);

	// Set all the particles to be updated by Verlet 
	// (i.e. they AREN'T being ignored)
	void ReleaseFlag(void);

private:
	sMesh* m_pViperPlayer = NULL;
	cSoftBodyVerlet* m_pFlag = NULL;

	// Edge of the flag vertex (particle) index location
	// std::vector< sParticle* > vec_pParticles;			
	std::vector< cSoftBodyVerlet::sParticle* > m_vecParticles;

};
