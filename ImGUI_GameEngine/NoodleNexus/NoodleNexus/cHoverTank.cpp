#include "cHoverTank.h"
#include <iostream>

cHoverTank::cHoverTank()
{
	std::cout << "A hover tank is created" << std::endl;
	this->m_ID = cHoverTank::m_NextID++;
	this->p_CurrentTarget = NULL;

}

/*static*/
unsigned int cHoverTank::m_NextID = 3000;


cHoverTank::~cHoverTank()
{

}

void cHoverTank::setMediator(iMessaging* pTheMediator)
{
	this->m_pTheMediator = pTheMediator;
	return;
}

//void cHoverTank::Attack(/*another tank*/)
void cHoverTank::Attack(iTank* pEnemyTank)
{

	return;
}

glm::vec3 cHoverTank::getLocation(void)
{
	// TODO: 

	return this->m_location;
}

void cHoverTank::setLocation(glm::vec3 newLocation_)
{
	this->m_location = newLocation_;
	return;
}


void cHoverTank::UpdateTick(double deltaTime)
{
	// TODO:
//	std::cout << "Tank# " << this->m_ID << " updating" << std::endl;

	return;
}

unsigned int cHoverTank::getID(void)
{
	return this->m_ID;
}

// From the iMessaging interface
// // Sent to the recieving object
bool cHoverTank::Receive_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}

// From the iMessaging interface
// // Sent to the mediator to pass along
bool cHoverTank::Send_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}