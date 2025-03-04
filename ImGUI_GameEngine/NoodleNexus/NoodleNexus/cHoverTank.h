#pragma once

#include "iTank.h"
#include "iMessaging.h"
#include <glm/glm.hpp>
#include "sMesh.h"
//
#include "cJetPack.h"
#include "cSheild.h"

class cHoverTank : 
	public iTank,
	public iMessaging
{
public:
	cHoverTank();
	virtual ~cHoverTank();

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

	cJetPack* pTheJetPack;
	cSheild* pShield;

	virtual unsigned int getID(void);

private:
	static unsigned int m_NextID;
	unsigned int m_ID;

	glm::vec3 m_location;

	// If NULL, there isn't a target
	// If ! NULL, then we are attacking something
	iTank* p_CurrentTarget;

	iMessaging* m_pTheMediator;

};