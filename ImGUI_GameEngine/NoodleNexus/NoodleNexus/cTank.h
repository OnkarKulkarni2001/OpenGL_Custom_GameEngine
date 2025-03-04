#pragma once

#include "iTank.h"
#include <glm/glm.hpp>
#include "sMesh.h"

// 
#include "cSheild.h"
#include "iMessaging.h"

class cTank : 
	public iTank, 
	public iMessaging
{
public:
	cTank();
	virtual ~cTank();

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

	cSheild* pTheShield;

	virtual unsigned int getID(void);

	float health;



private:
	static unsigned int m_NextID;
	unsigned int m_ID;

	glm::vec3 m_location;

	// If NULL, there isn't a target
	// If ! NULL, then we are attacking something
	iTank* p_CurrentTarget;


	double m_FireCoolDownTimer;

	enum eState
	{
		IDLE,
		MOVING,
		ATTACKING
	};
	eState m_CurrentState;

	iMessaging* m_pTheMediator;
};