#include "cSheild.h"

cSheild::cSheild()
{
	this->m_bIsRaised = false;
}

void cSheild::Raise(void)
{
	this->m_bIsRaised = true;
	return;
}

void cSheild::Lower(void)
{
	this->m_bIsRaised = false;
	return;
}

bool cSheild::bIsUp(void)
{
	return this->m_bIsRaised;
}



// From the iMessaging interface
// // Sent to the recieving object
bool cSheild::Receive_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}

// From the iMessaging interface
// // Sent to the mediator to pass along
bool cSheild::Send_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}