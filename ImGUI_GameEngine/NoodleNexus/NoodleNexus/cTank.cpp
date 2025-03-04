#include "cTank.h"
#include <iostream>

cTank::cTank()
{
	std::cout << "A regular tank is created" << std::endl;
	this->m_ID = cTank::m_NextID++;
	this->health = 100.0f;
	this->p_CurrentTarget = NULL;
	// 
	this->m_CurrentState = eState::IDLE;
}

/*static*/
unsigned int cTank::m_NextID = 1000;

unsigned int cTank::getID(void)
{
	return this->m_ID;
}


cTank::~cTank()
{

}

//void cTank::Attack(/*another tank*/)
void cTank::Attack(iTank* pEnemyTank)
{

	return;
}

glm::vec3 cTank::getLocation(void)
{
	// TODO: 

	return this->m_location;
}

void cTank::setLocation(glm::vec3 newLocation_)
{
	this->m_location = newLocation_;
	return;
}


void cTank::UpdateTick(double deltaTime)
{
	// TODO:
//	std::cout << "Tank# " << this->m_ID << " updating" << std::endl;

	switch (this->m_CurrentState)
	{
	case eState::IDLE:
		// Immediately attack
		this->m_CurrentState = eState::ATTACKING;
		break;
	case eState::ATTACKING:
		 
		if (this->m_FireCoolDownTimer <= 0.0)
		{
			// We can fire again
			// Find a target location (FindFarthestTank)
			sNVPair message;
			message.theCommand = "FindFarthestTank";
			message.vecDetails.push_back(glm::vec4( this->m_location, 1.0f));

			this->m_pTheMediator->Receive_Message(message);
			// Shoot at it
		}
		else
		{
			this->m_FireCoolDownTimer -= deltaTime;
		}
		break;

	case eState::MOVING:

		break;

	default:
		// What's going on?? 
		// Maybe log an error?? 
		break;
	}

	return;
}

void cTank::setMediator(iMessaging* pTheMediator)
{
	this->m_pTheMediator = pTheMediator;
	return;
}

// From the iMessaging interface
// // Sent to the recieving object
bool cTank::Receive_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}

// From the iMessaging interface
// // Sent to the mediator to pass along
bool cTank::Send_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}

