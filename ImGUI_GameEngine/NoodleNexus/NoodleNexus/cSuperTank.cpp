#include "cSuperTank.h"

#include <iostream>

cSuperTank::cSuperTank()
{
	std::cout << "A super tank is created" << std::endl;
	this->m_ID = cSuperTank::m_NextID++;
	//// Add a rail gun
	//this->pTheRailGun = new cRailGun();
	this->p_CurrentTarget = NULL;

}

/*static*/
unsigned int cSuperTank::m_NextID = 8000;


cSuperTank::~cSuperTank()
{
//	delete this->pTheRailGun;
}

void cSuperTank::setMediator(iMessaging* pTheMediator)
{
	this->m_pTheMediator = pTheMediator;
	return;
}

//void cSuperTank::Attack(/*another tank*/)
void cSuperTank::Attack(iTank* pEnemyTank)
{
	if (this->p_CurrentTarget != NULL)
	{
		this->pTheRailGun->Shoot(pEnemyTank);
	}

	return;
}

glm::vec3 cSuperTank::getLocation(void)
{
	// TODO: 

	return this->m_location;
}

void cSuperTank::setLocation(glm::vec3 newLocation_)
{
	this->m_location = newLocation_;
	return;
}

void cSuperTank::UpdateTick(double deltaTime)
{
	// TODO:
//	std::cout << "Tank# " << this->m_ID << " updating" << std::endl;

	// Is there a target? 
	if (this->p_CurrentTarget == NULL)
	{
		// Nope, so pick a new target
		// How do we do this??? 
	}
	else
	{
		// Rail gun is the thing that actually hits the tank
		this->pTheRailGun->Shoot(this->p_CurrentTarget);
	}

	return;
}

unsigned int cSuperTank::getID(void)
{
	return this->m_ID;
}



// From the iMessaging interface
// // Sent to the recieving object
bool cSuperTank::Receive_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}

// From the iMessaging interface
// // Sent to the mediator to pass along
bool cSuperTank::Send_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}