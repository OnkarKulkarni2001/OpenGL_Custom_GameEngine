#pragma once

#include "iTank.h"
#include "iMessaging.h"
#include <glm/glm.hpp>
#include "sMesh.h"
//
#include "cSheild.h"
#include "cRailGun.h"

class cSuperTank : 
	public iTank,
	public iMessaging
{
public:
	cSuperTank();
	virtual ~cSuperTank();

	// ********************************
	// From the iMessaging interface
	//		This method is called BY the mediator
	virtual bool Receive_Message(sNVPair theMesssage);
	//		Normally this is what we sent to the mediator
	virtual bool Send_Message(sNVPair theMesssage);
	// ********************************


	//	void Attack(/*another tank*/);
	virtual void Attack(iTank* pEnemyTank);
	virtual glm::vec3 getLocation(void);
	virtual void setLocation(glm::vec3 newLocation_);
	virtual void UpdateTick(double deltaTime);
	virtual void setMediator(iMessaging* pTheMediator);


	sMesh* pTheMesh;

	// Now with 2 shields
	cSheild* pTheShield[2];
	cRailGun* pTheRailGun;

	virtual unsigned int getID(void);

private:
	static unsigned int m_NextID;
	unsigned int m_ID;

	glm::vec3 m_location;

	// If NULL, there isn't a target
	// If ! NULL, then we are attacking something
	iTank* p_CurrentTarget;

	std::vector< iTank* > m_vec_AllTheTanks;

	iMessaging* m_pTheMediator;


};